#ifndef CPPCHAT_CLIENT_HPP
#define CPPCHAT_CLIENT_HPP

#include <string>
#include "handler.hpp"
#include "net_util.hpp"
#include "message.hpp"

namespace cppchat {

using namespace std;

class Client {
    public:
    Client(string host, int port, Handler &handler) : sockFd_(-1), handler_(handler) {
        sockFd_ = CreateAndConnectSocket(host, port);
    }
    ~ Client() {}

    void start() {
        while(true) {
             handler_.authorize(sockFd_);
             // 新开线程处理接收函数
             pthread_t newthread;
             if(pthread_create(&newthread, NULL, &handler_.output, (void*)&sockFd_ ) != 0) {
                 LogError("pthread_create error!");
                 exit(1);
             }
             // 处理输入
             handler_.input(sockFd_);
        }
    }

    private:
    int sockFd_;
    Handler &handler_;
};

}

#endif

