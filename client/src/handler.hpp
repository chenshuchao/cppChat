#ifndef CPPCHAT_HANDLER_HPP
#define CPPCHAT_HANDLER_HPP

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "limonp/Logger.hpp"
#include "net_util.hpp"
#include "constant.hpp"
#include "message.hpp"

namespace cppchat {
using namespace std;
using namespace rapidjson;

class Handler {
public:
    Handler() {}
    ~ Handler() {}
    
    bool authorize(int sockfd) {
        while(true) {
            cout << "请按 1 登录, 或按 2 注册 : ";
            int opt;
            cin >> opt;
            cout << endl;
            if(1 == opt) {
                string account, password;
                cout << "请输入帐号: ";
                cin >> account;
                cout << "请输入密码: ";
                cin >> password;
                cout << endl;
                if(login(sockfd, account, password)) {
                    account_ = account;
                    cout << "登录成功～" << endl;
                    break;
                } else {
                    cout << "登录失败～" << endl;
                }
            } else if(2 == opt) {
                string account, password1, password2;
                cout << "请输入要注册的账号: ";
                cin >> account;
                do {
                    cout << "请输入密码: ";
                    cin >> password1;
                    cout << "请重复密码: ";
                    cin >> password2;
                    if(!password1.empty() && password1 == password2) break;
                } while(true);
                if(register_account(sockfd, account, password1)) {
                    cout << "注册成功~" << endl;
                } else {
                    cout << "注册失败~" << endl;
                }
            }
        }
        return true;
    }

    static void* output(void* sockfd1) {
        int sockfd = *(int *)sockfd1;
        while(true) {
            string strRecv;
            recvMsg(sockfd, strRecv);
            Document d;
            d.Parse(strRecv.c_str());
            if(d.HasMember("type")) {
                int opt = stoi(d["type"].GetString());
                switch(opt) {
                    case TYPE_LOGOUT:
                        if(!handleLogout(d)) {
                            LogError("handle logout error");
                        }
                        break;
                    case TYPE_CHAT:
                        if(!handleChat(d)) {
                            LogError("handle chat error");
                        }
                        break;
                    default:
                        LogInfo("unknown type of pack");
                } 
            }
        }
        return NULL;
    }
    void input(int sockfd) {
        while(true) {
            string words;
            getline(cin, words);
            if(words == "LOGOUT") {
                mode_ = TYPE_LOGOUT;
                continue;
            } else if(words == "CHAT") {
                mode_ = TYPE_CHAT;
                continue;
            }
            string strSend; 
            switch(mode_) {
                case TYPE_CHAT:
                    strSend = genChatMsg(account_, words);
                    break;
                case TYPE_LOGOUT:
                    strSend = genLogoutMsg(account_);
                    break;
                default:
                    LogInfo("unknown type of input");
            }
            sendMsg(sockfd, strSend);
        }
    }
 
private:
    int mode_;
    string account_;

    bool login(int sockfd, const string &account, const string &password) {
        string msg = genLoginMsg(account, password);
        if(sendMsg(sockfd, msg)) {
            string resp;
            if(recvMsg(sockfd, resp)) {
                Document d;
                d.Parse(resp.c_str());
                if(d.HasMember("type") && d.HasMember("status")
                    && TYPE_LOGIN == stoi(d["type"].GetString()) && stoi(d["status"].GetString()) == SUCCESS) {
                        return true;
                }
            }
        }
        return false;
    }
    bool register_account(int sockfd, const string &account, const string &password) {
        string msg = genRegisterMsg(account, password);
        if(sendMsg(sockfd, msg)) {
            string resp;
            if(recvMsg(sockfd, resp)) {
                Document d;
                d.Parse(resp.c_str());
                if(d.HasMember("type") && d.HasMember("status")
                    && TYPE_REGISTER == stoi(d["type"].GetString()) && stoi(d["status"].GetString()) == SUCCESS) {
                        return true;
                }
            }
        }
        return false;
    }
    static bool handleLogout(Document &d) {
        return true;
    }
    static bool handleChat(Document &d) {
        if(!d.HasMember("sender") || !d.HasMember("content")) {
            return false;
        }
        string sender = d["sender"].GetString();
        string content = d["content"].GetString();
        cout << sender << ": " << content << endl;
        return true;
    }
    static bool sendMsg(int sockfd, const string& strSnd) {
        if(-1 == send(sockfd, strSnd.c_str(), strSnd.length(), 0)) {
            LogError(strerror(errno));
            return false;
        }
        return true;
    }
    static bool recvMsg(int sockfd, string &msg) {
        char recvBuf[RECV_BUFFER_SIZE];
        int n;
        if((n = recv(sockfd, recvBuf, RECV_BUFFER_SIZE, 0)) > 0) {
            msg = string(recvBuf, n);
            LogDebug("recv msg: %s", msg.c_str());
            return true;
        }
        LogDebug("recv msg error");
        return false;
    }
};

}
#endif

