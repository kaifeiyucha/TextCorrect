#ifndef CACHE_H_
#define CACHE_H_ 

#include <iostream>
#include <map>
#include <list>
#include <utility>
#include <string>
#include <set>


class LRUCache{
    public:
//        LRUCache();
        LRUCache(int size)
        :size_(size)
        {
            exit_ = false;
            size_ = size;
        }

        std::string get_from_cache(std::string key);// 从cache中取出结果
        void put_into_cache(std::string key, std::set<std::string> value);//把结果放到cache中
        void read_cache_file(const std::string &filename);
        void write_cache_file(const std::string &fiename);
        void set_order(size_t order) {order_ = order;}//设置cache的编号
        size_t get_order(){ return order_;}//得到cache池中的cache的编号
        void print_cache();//test函数
        //std::map<std::string, std::string> &get_cache(){   return cachesMap_;}
    private:
        bool exit_;
        size_t size_;//cache的最大容量
        size_t order_; //用来标记cache池中的cache序号
        std::list<std::pair<std::string, std::string> > caches_; //用双端队列存储cache中的内容
        std::map< std::string, std::list<std::pair<std::string, std::string> >::iterator> cachesMap_;//用map加快查找速度
};
#endif  /*CACHE_H_*/
