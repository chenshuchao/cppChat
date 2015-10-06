#ifndef CPPCHAT_THREADPOOLSERVER_H
#define CPPCHAT_THREADPOOLSERVER_H

#include "net_util.hpp"
#include "worker_thread.hpp"

namespace cppchat {
using namespace limonp;

class ThreadPoolServer {
    public:
        ThreadPoolServer(size_t thread_number, size_t queue_max_size, size_t port, Handler &handler):
            pool_(thread_number, queue_max_size), req_handler_(handler), host_socket_(-1) {
            host_socket_ = CreateAndListenSocket(port);
        }
        ~ThreadPoolServer() {};

        bool start() {
            LogInfo("server start...");
            pool_.start();
            sockaddr_in clientaddr;
            socklen_t nSize = sizeof(clientaddr);
            int clientSock;

            while(true) {
                if(-1 == (clientSock = accept(host_socket_, (struct sockaddr*) &clientaddr, &nSize))) {
                    LogError(strerror(errno));
                    break;
                }
                pool_.add(CreateTask<WorkerThread,int,Handler&>(clientSock, req_handler_));
            }
            return true;
        }

    private:
        ThreadPool pool_;
        Handler & req_handler_;
        int host_socket_;
};
}

#endif

