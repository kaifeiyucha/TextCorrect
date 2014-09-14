#ifndef SERVER_H
#define SERVER_H 
#include <echo/TcpServer.h>
#include <echo/ThreadPool.h>
#include "query.h"
#include "get_config.h"
#include "Cache.h"
//#include "cacheManager.h"

class QueryServer:NonCopyable{
    public:        
        QueryServer(GetConfig *config);
        ~QueryServer();
        void start();

    private:
        void onConnection(const TcpConnectionPtr &);
        void onMessage(const TcpConnectionPtr &);
        void closeConnection(const TcpConnectionPtr &c);
        void runQuery(const std::string &, const TcpConnectionPtr &);
        
        TcpServer server_;
        Query query_;
        ThreadPool pool_;
        LRUCache cache_;
//        CacheManager caches_;
};
#endif  /*SERVER_H*/
