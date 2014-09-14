#include "QueryServer.h"
#include "Cache.h"
#include "get_config.h"
#include <string>
#include <sstream>
#include <iostream>

using namespace std;
using namespace std::placeholders;

QueryServer::QueryServer(GetConfig *config)
    :server_(InetAddress(config->getPort())),
     //query_(config->getDicEnFile, config->getDicChFile()),
    query_("DicEn.txt","DicCh.txt"),
    pool_(config->getQueueSize(), config->getPoolSize()),
    cache_(100)
{
    server_.setConnection(bind(&QueryServer::onConnection, this, _1));
    server_.setMessage(bind(&QueryServer::onMessage, this, _1));
    server_.setClose(bind(&QueryServer::closeConnection, this, _1));

}

QueryServer::~QueryServer(){
   // cache_.write_cache_file("../data/Cache.txt");
    pool_.stop();
    //caches_.stop();

}

//开始连接
void QueryServer::onConnection(const TcpConnectionPtr &conn){
    ostringstream os;
    os << "client " << conn->getPeerAddr().toIp() <<":" << conn->getPeerAddr().toPort() << " is online" << endl;
    
   
    conn->send("please input word:\r\n");
}


void QueryServer::onMessage(const TcpConnectionPtr &conn){

    string word = conn->receive();
    
    //给线程池增加一个任务
     pool_.addTask(bind(&QueryServer::runQuery, this, word, conn));
}

//关闭连接
void QueryServer::closeConnection(const TcpConnectionPtr  &conn){
    ostringstream os;
    os << "client " << conn->getPeerAddr().toIp() << ":" << conn->getPeerAddr().toPort() << "is offline." << endl;
}

void QueryServer::runQuery(const string &s, const TcpConnectionPtr  &conn){
    
    string word = s;
    if(word.substr(word.size()-2, 2)=="\r\n"){
        word.erase(word.size()-1);
        word.erase(word.size()-1);
    }
    string res = query_.run_query(word);
    conn->send(res + "\r\n");
}

void QueryServer::start(){
    pool_.start();
    server_.start();
}

