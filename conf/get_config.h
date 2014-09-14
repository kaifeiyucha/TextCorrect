#ifndef GETCONFIG_H_
#define GETCONFIG_H_ 

#include <pthread.h>
#include <iostream>
#include <string>
#include <stdlib.h>

class GetConfig{
    public:
        static GetConfig* getInstance(){
            //获取对象
            pthread_once(&once_, &initInstance);
            return pInstance_;
        }
        static void initInstance(){
            ::atexit(&destroyInstance);//注册销毁函数
            pInstance_ = new GetConfig;
        }
        static void destroyInstance(){
            std::cout << "destroy" << std::endl;
            delete pInstance_;
        }

        //下面的函数就是具体解析conf文件的函数
        void readConfig(const std::string &filename);

        int getPort() {return port_;}
        const std::string &getDicEnFile()const{ return dicen_; }
        const std::string &getDicChFile()const { return dicch_; }
        int getCacheNum(){ return cachenum_;}
        size_t getQueueSize(){  return queuesize_;}
        size_t getPoolSize(){  return poolsize_;}
    private:
        GetConfig(){};
        GetConfig(const GetConfig &);
        void operator=(const GetConfig &);

        static GetConfig *pInstance_;
        static pthread_once_t once_;

        int port_;
        std::string logfile_;
        std::string cache_; 
        std::string dicen_;
        std::string dicch_;
        int cachenum_;
        size_t  queuesize_;
        size_t poolsize_;
};
#endif  /*GETCONFIG_H_*/
