#ifndef CPP_NETUTIL_HPP
#define CPP_NETUTIL_HPP

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string>
#include "limonp/Logger.hpp"

namespace cppchat {
using namespace std;

typedef int SocketFd;

inline SocketFd CreateAndConnectSocket(string host, int port) {
    SocketFd sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sock) {
        LogError("create socket failed");
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host.c_str());
    addr.sin_port = htons(port);
    if(-1 == connect(sock, (struct sockaddr *)&addr, sizeof(addr))) {
        LogFatal(strerror(errno));
    }

    return sock;
}

}

#endif

