#include "CacheManager.h"
#include <iostream>

using namespace std;

CacheManager::CacheManager(const string &cachefile, size_t cacheNum, int updateFrequence)
    :cachefile_(cachefile),
     cacheNum_(cacheNum),
     flags_(cacheNum),
     isStarted_(true),
     empty_(mutex_),
     full_(mutex_)
{
    //加载Cachepool的文件
    cacheManager_.read_cache_file(filename);

    //加载每个CacheManager块,初始化caches_
    map<string ,string>::iterator itbeg=cacheManager_.get_cache.begin();
    map<string ,string>::iterator itend=cacheManager_.get_cache.end();
    for(vector<LRUCache>::iterator it=caches_.begin();it != cahces_.end(); ++it){
        it->get_cache.insert(itbeg, itend);
    }

    //设置Caches_的编号
    for(vector<LRUCache>::size_type ix=0; ix!=caches_.size(); ++ix){
        caches_[ix] = set_order(ix);//初始化order
        flags_[ix] = 0;//初始化flags_
        queue_.enqueue(ix);//初始化队列
    }
    
    //设置定时器
    timer_.setTimer(10, updateFrequence);
    timer_.setFunc(::bind(&CacheManger::write_cache_file, this));
}


CacheManager::~CacheManager(){
    timer_.stop();
    stop();
}


void CacheManager::start(){
    timer_.start();
}

void CacheManager::stop(){
    if(isStarted_){
        isStarted_ = true;
        full_.broadcast();
    }
}

LRUCache *CacheManager::get_cache(){
    MutexGuade lock(mutex_);
    while(isStarted_ && queue_.isEmpty()){
        full_.wait();//没有可工作的Cache
    }
    //空闲Cache 出队列去工作
    size_t i;
    if(!queue_.isEmpty()){
        queue_.dequeue(i);
    }

    return &caches_[i];
}

void CacheManager::return_cache(size_t i){
    MutexGuade lock(mutex_);
    queue_.enqueue(i);

    //当有多余一个的LRUCache的时候，代表没有等待的线程
    //所以只需，仅当有一个空闲的的LRUCache时，再给线程发full就可以
    if(queue_.getSize() == 1){
        full_.signal();
    }
}

void CacheManager::write_to_file(){
    map<string, list< pair<string,string> >::iterator> ::iterator begin,end;
    //遍历整个cacheManager
    for(size_t i =0; i< cacheNum_; i++){
        //遍历每个Cache块中的词条
        begin = cacheManager_.get_cache().begin();
        end  = cacheManager_.get_cache().end();
        cacheManager_.get_cache().insert(begin, end);
    }

    //写回Cache文件
    cacheManager_.write_cache_file(cachefile_);
    update_cache();//更新与Caches_有关的模块
}

void CacheManager::update_cache(){
    size_t i=0;
    map<string, list< pair<string,string> >::iterator> ::iterator begin,end;
    begin = cacheManager_.get_cache().begin();
    end = cacheManager_.get_cache().end();
    //遍历cacheManger
    while( i!= cacheNum_){
        LRUCache *mycache = get_cache();
        size_t order = mycache->get_order();
        if(flags_[order] == 0){//把没有活动的cacheManaaer激活
            mycache->get_cache().insert(begin, end);
            flags_[order] = 1;
        }
        //归还工作的Cache
        return_cache(order);
        if(order == i)
            ++i;
    }
    //更新完毕后，把flags_恢复初值
        for(vector<size_t>::iterator it = flags_.begin(); it!=flags_.end(); ++it){
            *it = 0;
        }

        cout << "update cache " << endl;
}
