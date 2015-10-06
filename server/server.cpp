#define LOGGER_LEVEL LL_INFO

#include <stdio.h>
#include "limonp/Config.hpp"
#include "thread_pool_server.hpp"

using namespace cppchat;
int main(int argc, char *argv[]) {
    if(argc < 2) {
        printf("usage: %s <config_file>\n", argv[0]);
        return 1;
    }
    Config conf(argv[1]);
    int port = conf.get("port", 5011);
    int threadNumber = conf.get("thread_number", 4);
    int queueMaxSize = conf.get("queue_max_size", 1024);
   
    string redis_host = conf.get("redis_host", "127.0.0.1");
    string redis_port = conf.get("redis_port", "6379");
    string redis_password = conf.get("redis_password", "");
    string user_prefix = conf.get("user_prefix", "");

    Handler handler(redis_host, redis_port, redis_password, user_prefix);
    ThreadPoolServer s(threadNumber, queueMaxSize, port, handler);
    s.start();

    return 0;
}

