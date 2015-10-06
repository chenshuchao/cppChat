#ifndef CPPCHAT_HANDLER_HPP
#define CPPCHAT_HANDLER_HPP

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "limonp/Logger.hpp"
#include "db_operator.hpp"
#include "constant.hpp"

namespace cppchat {
class Handler {
    public:
        Handler(const string &redis_host, const string &redis_port, const string &redis_password, const string &user_prefix) :
            DBOperator_(redis_host, redis_port, redis_password, user_prefix, "online") {
        }
        ~ Handler() {}

        bool handleRegister(const Document &d, string &response) {
            LogInfo("recv register pack");
            if(!d.HasMember("account") || !d.HasMember("password")) {
                response = genResponse(TYPE_REGISTER, FAIL, "参数错误"); 
                return false;
            }

            string account = d["account"].GetString();
            string password = d["password"].GetString();
            try {
                // 检查用户是否存在
                if(DBOperator_.isExisted(account) ) {
                    response = genResponse(TYPE_REGISTER, FAIL, "用户已经存在。");
                    return false;
                }
                if(DBOperator_.addUser(account, password)) {
                    response = genResponse(TYPE_REGISTER, SUCCESS, "");
                    return true;
                }
            } catch(exception &e) {
                LogError(e.what());
            }
            response = genResponse(TYPE_REGISTER, FAIL, "");
            return false; 
        }
        bool handleLogin(const Document &d, string &response, int sockfd) {
            LogInfo("recv login pack");
            if(!d.HasMember("account") || !d.HasMember("password")) {
                response = genResponse(TYPE_LOGIN, FAIL, "参数错误"); 
                return false;
            }

            string account = d["account"].GetString();
            string password = d["password"].GetString();
            try {
                if(DBOperator_.getUserAtrr(account, "password") == password && DBOperator_.setOnline(account, sockfd)) {
                    response = genResponse(TYPE_LOGIN, SUCCESS, ""); 
                    return true;
                }
            } catch(exception &e) {
                LogError(e.what());
            } catch(...) {
                LogError("unknown error");
            }
            response = genResponse(TYPE_LOGIN, FAIL, ""); 
            return false;
        }
        bool handleLogout(const Document &d, string &response) {
            LogInfo("recv logout pack");
            if(!d.HasMember("account")) {
                response = genResponse(TYPE_LOGOUT, FAIL, "参数错误"); 
                return false;
            }

            string account = d["account"].GetString();
            if(DBOperator_.setOffline(account)) {
                response = genResponse(TYPE_LOGOUT, SUCCESS, "");
                return true;
            }
            response = genResponse(TYPE_LOGOUT, FAIL, ""); 
            return false;
        }
        bool handleClose(string account, int sockfd) {
            LogInfo("handle close");
            if(!account.empty()) {
                try {
                    DBOperator_.setOffline(account);
                } catch(...) {
                    LogError("set offline exception");
                }
            }
            LogDebug("close sockfd: %d", sockfd);

            if(-1 == close(sockfd)) {
                LogError(strerror(errno));
            }
            return true;
        }
        bool handleChat(const Document &d, string &response) {
            LogInfo("recv chat pack");
            if(!d.HasMember("sender") || !d.HasMember("content")) {
                response = genResponse(TYPE_CHAT_RESP, FAIL, "参数错误");
                return false;
            }

            string sender = d["sender"].GetString();
            string content = d["content"].GetString();
            unordered_map<string, int> m;
            if(DBOperator_.getOnlineUser(m)) {
                string msg = genChatMsg(TYPE_CHAT, sender, content);
                for(unordered_map<string, int>::iterator it = m.begin(); it != m.end(); it ++) {
                    LogDebug("send to %s", (it->first).c_str());
                    if(it->first != sender) {
                        sendMessage(it->second, msg);
                    }
                }
                return true;
            }
            return false;
        }

    private:
        DBOperator DBOperator_;
        bool sendMessage(int sockfd, const string& strSnd) const {
            LogDebug("send msg: %s", strSnd.c_str());
            if(-1 == send(sockfd, strSnd.c_str(), strSnd.length(), 0)) {
                LogError(strerror(errno));
                return false;
            }
            return true;
        }
};

}
#endif

