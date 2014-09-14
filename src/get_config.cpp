#include "get_config.h"
#include <fstream>
#include <stdexcept>

using namespace std;

GetConfig *GetConfig::pInstance_ = NULL;
pthread_once_t GetConfig::once_ = PTHREAD_ONCE_INIT;

void GetConfig::readConfig(const string &filename){
    ifstream infile;
    infile.open(filename.c_str());
    if(!infile){
        throw runtime_error("open config file failed!");
    }
    
    string line;
    while(infile >> line){
        string type;
        string param; //参数
        string::size_type pos;
        pos = line.find("=");
        type = line.substr(0,pos);
        param = line.substr(pos+1);

//        cout << "type: " << type << "     param: " << param << endl;
        
        if(type == "port")
            port_ = atoi(param.c_str());
        if(type == string("queueSize"))
            queuesize_ = atoi(param.c_str());
        if(type == string("DicEn"))
            dicen_ = param;
        if(type == string("DicCh"))
            dicch_ = param;
        if(type == string("cachenum"))
            cachenum_ = atoi(param.c_str());
        if(type == string("poolSize"))
            poolsize_ = atoi(param.c_str());
    }
        cout << "port_: " << port_ << endl;

}
