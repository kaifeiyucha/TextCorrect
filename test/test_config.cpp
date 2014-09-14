#include "get_config.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, const char *argv[])
{
//    GetConfig::initInstance();
    GetConfig  *conf=GetConfig::getInstance();

    conf->readConfig(string("../conf.txt"));

    return 0;
}
