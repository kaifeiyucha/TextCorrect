#ifndef CACHEMANAGER
#define CACHEMANAGER 

#include "Cache.h"
#include "Mutex.h"
#include "Cond.h"
#include "ThreadTimer.h"
#include "Queue.hpp"
#include <string>
#include <vector>

class CacheManager{
    public:
        CacheManager(const std::string &cachefile, size_t cacheNum, int updateFrequence);
        ~CacheManager();

        void start();
        void stop();
        LRUCache *get_cache();//得到可用cache块
        void return_cache(size_t i);//把用过的Cache块归还
        void update_cache();//更新Cache文件的相关模块
        void write_to_file();//把更新后的内容写到Cache文件中

    private:
        bool isStarted_;//判断CachePool是否开启
        size_t cacheNum_;//CachePool的大小
        std::string cachefile_;//要加载的Cache文件名字
        std::vector<LRUCache> caches_;//Cache管理器的数据结构
        LRUCache  cacheManager_;//CacheManager管理器变量
        ThreadTimer timer_;//定时器
        std::vector<size_t> flags_;//判断Cache块忙闲状态的数组
        Queue<size_t> queue_;//用来装可用Cache块编号
        Mutex mutex_;
        Cond full_;
        Cond empty_;



};

#endif  /*CACHEMANAGER*/
