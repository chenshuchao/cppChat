#ifndef CPPCHAT_WORKER_HPP
#define CPPCHAT_WORKER_HPP

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "limonp/ThreadPool.hpp"
#include "net_util.hpp"
#include "message.hpp"
#include "constant.hpp"
#include "handler.hpp"

namespace cppchat {
using namespace limonp;
using namespace rapidjson;

const size_t RECV_BUFFER_SIZE = 16 * 1024;

class WorkerThread: public ITask {
    public:
        WorkerThread(int sockfs, Handler& reqHandler):
            sockfd_(sockfs), req_handler_(reqHandler) {
        }
        virtual ~WorkerThread() {
        }
        virtual void run() {
            string account;
            while(true) {
                string strReq, strResp;
                // 读消息失败，断开连接
                if(!receive(strReq)) break;

                Document d;
                d.Parse(strReq.c_str());
                if(d.HasMember("type")) {
                    int opt = stoi(d["type"].GetString());
                    switch(opt) {
                        case TYPE_LOGIN:
                            if(!req_handler_.handleLogin(d, strResp, sockfd_)) {
                                LogError("handle login error");
                            } else {
                                account = d["account"].GetString();
                            }
                            break;
                        case TYPE_LOGOUT:
                            if(!req_handler_.handleLogout(d, strResp)) {
                                LogError("handle logout error");
                            }
                            break;
                        case TYPE_REGISTER:
                            if(!req_handler_.handleRegister(d, strResp)) {
                                LogError("handler register error");
                            }
                            break;
                        case TYPE_CHAT:
                            if(!req_handler_.handleChat(d, strResp)) {
                                LogError("handle chat error");
                            }
                            break;
                        default:
                            LogInfo("unknown type of pack");
                    } 
                }
                if(!strResp.empty()) {
                    sendMessage(strResp);
                }
            }
            
            req_handler_.handleClose(account, sockfd_);
        }
    private:
        bool receive(string &msg) const {
            char recvBuf[RECV_BUFFER_SIZE];
            int n = 0;
            if((n = recv(sockfd_, recvBuf, RECV_BUFFER_SIZE, 0)) <= 0) {
                LogError(strerror(errno));
                return false;
            } 
            msg = string(recvBuf, n);
            LogInfo(msg.c_str());
            return true;
        }
        bool sendMessage(const string& strSnd) const {
            if(-1 == send(sockfd_, strSnd.c_str(), strSnd.length(), 0)) {
                LogError(strerror(errno));
                return false;
            }
            return true;
        }

        int sockfd_;
        string account_;
        Handler& req_handler_;
};

}
#endif

