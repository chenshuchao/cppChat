#ifndef CPPCHAT_USER_HPP
#define CPPCHAT_USER_HPP

#include <string>
#include <unordered_map>
#include "redispp/redispp.h"

namespace cppchat {
using namespace std;
using namespace redispp;

class DBOperator {
    public:
        DBOperator(const string &redis_host, const string &redis_port, const string &redis_password, 
                   const string &prefix, const string &online):
            conn_(redis_host, redis_port, redis_password), userPrefix_(prefix), online_(online) {
        }
        ~ DBOperator() {
        }
        bool isExisted(const string &id) {
            if(conn_.exists(userPrefix_ + id)) {
                LogDebug("is Existed");
                return true;
            }
            LogDebug("is not Existed");
            return false;
        }
        bool addUser(const string &id, const string &password) {
            return conn_.hset(userPrefix_ + id, "password", password);
        }
        string getUserAtrr(const string &id, const string &key) {
            return (string)conn_.hget(userPrefix_ + id, key);
        }
        bool setOnline(const string &id, int sock) {
            return conn_.hset(online_, id, to_string(sock));
        }
        bool setOffline(const string &id) {
            return conn_.hdel(online_, id);
        }
        int getOnlineUserSock(const string &id) {
            return stoi((string)conn_.hget(online_, id));
        }
        bool getOnlineUser(unordered_map<string, int> &m) {
            MultiBulkEnumerator result = conn_.hgetAll(online_);
            string str1, str2;
            while(result.next(&str1) && result.next(&str2)) {
                LogDebug("str1: %s, str2: %s", str1.c_str(), str2.c_str());
                m.insert(pair<string, int>(str1, stoi(str2)));
            }
            return true;
        }
        
    private:
        Connection conn_;
        string userPrefix_;
        string online_;
};

}
#endif

