#include "Cache.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <assert.h>

using namespace std;

void LRUCache::read_cache_file(const string &filename){
    ifstream infile;
    infile.open(filename.c_str());
    if(!infile){
        throw runtime_error("open cache file to read failed!");
    }
    string line;
    string usrword;
    string libword;
    while(getline(infile, line)){
         istringstream iss(line);
         iss >> usrword >> libword;
      //   caches_.insert(make_pair(usrword, libword));
         caches_.push_back(make_pair(usrword, libword));
         
    }
    infile.close();
    infile.clear();
    
}

void LRUCache::write_cache_file(const string &filename){
    fstream outfile;
    outfile.open(filename.c_str());
    if(!outfile){
        throw runtime_error("open cache file to write failed!");
    }
    map<string, list< pair<string, string> >::iterator >::iterator it = cachesMap_.begin();
    for( ; it != cachesMap_.end(); ++it){
        outfile << it->first << " " << it->second->second << endl;
    }
    outfile.close();
    outfile.clear();
}

string LRUCache::get_from_cache(string key){
    string retValue = "";
    map<string, list< pair<string , string> > ::iterator> ::iterator it = cachesMap_.find(key);

    if(it != cachesMap_.end()){
        retValue = it->second->second;
        //移动到最前端
        list< pair<string, string> >::iterator ptrPair = it->second;
        pair<string ,string> temPair = *ptrPair;
        caches_.erase(ptrPair);
        caches_.push_front(temPair);
        //修改map中的值
        cachesMap_[key] = caches_.begin();
    }
    return retValue;
}


void LRUCache::put_into_cache(string key, set<string> value){
    map<string, list<pair<string, string> >::iterator>::iterator it = cachesMap_.find(key);
    if(it != cachesMap_.end()){
        list<pair<string, string> >::iterator ptrPair = it->second;
        
        string temp="";
        set<string>::iterator itSet=value.begin();//拼接字符串
        for(; itSet!=value.end(); ++itSet){
            temp+= ((*itSet)+" ");
        }

        ptrPair->second = temp;
        pair<string, string> temPair = *ptrPair;
        caches_.erase(ptrPair);
        caches_.push_front(temPair);//从头插入

        //更新map
        cachesMap_[key] = caches_.begin();
    }else{//在cache中没有这个词条
        string tempStr="";
        set<string>::iterator itSet=value.begin();
        for(; itSet!=value.end(); ++itSet){
            tempStr+=((*itSet)+"  ");
        }
        
        pair<string, string> temPair = make_pair(key,tempStr);
        
        if(size_ == caches_.size()){//已满
            string delKey = caches_.back().first;
            caches_.pop_back();//删除最后一个

            //删除在map中的相应项
            map<string, list<pair<string, string> >::iterator> ::iterator delIt = cachesMap_.find(delKey);
            cachesMap_.erase(delIt);
        }

        caches_.push_front(temPair);
        cachesMap_[key] = caches_.begin();//更新map,并放在开头
    
    }
    
}



