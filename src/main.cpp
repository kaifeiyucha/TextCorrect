#include "query.h"
#include "QueryServer.h"
#include "get_config.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, const char *argv[])
{
    GetConfig *config = GetConfig::getInstance();
    config->readConfig("../conf/conf.txt");
    QueryServer server(config);
    server.start();
    return 0;
}
