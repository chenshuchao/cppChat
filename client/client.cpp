#define LOGGER_LEVEL LL_INFO

#include "limonp/Config.hpp"
#include "client.hpp"
#include "handler.hpp"

using namespace limonp;
using namespace cppchat;

int main(int argc, char *argv[]) {
    if(argc < 2) {
        printf("usage: %s <config_file>\n", argv[0]);
        return 1;
    }
    Config conf(argv[1]);
    string host = conf.get("host", "localhost");
    int port = conf.get("port", 5011);

    Handler handler;
    Client c(host, port, handler);
    c.start();
    return 0;
}

