#ifndef CPPCHAT_MESSAGE_HPP
#define CPPCHAT_MESSAGE_HPP

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
        
#include "limonp/StringUtil.hpp"
#include "limonp/Logger.hpp"

#include "constant.hpp"

namespace cppchat {
using namespace std;
using namespace limonp;
using namespace rapidjson;

string genLoginMsg(const string &account, const string &password) {
    return string_format("{\"type\":\"%s\",\"account\":\"%s\",\"password\":\"%s\"}", to_string(TYPE_LOGIN).c_str(), account.c_str(), password.c_str());
}
string genLogoutMsg(const string &account) {
    return string_format("{\"type\":\"%s\",\"account\":\"%s\",\"password\":\"%s\"}", to_string(TYPE_LOGOUT).c_str(), account.c_str());
}
string genRegisterMsg(const string &account, const string &password) {
    return string_format("{\"type\":\"%s\",\"account\":\"%s\",\"password\":\"%s\"}", to_string(TYPE_REGISTER).c_str(), account.c_str(), password.c_str());
}
string genChatMsg(const string &account, const string &content) {
    return string_format("{\"type\":\"%s\",\"sender\":\"%s\",\"content\":\"%s\"}", to_string(TYPE_CHAT).c_str(), account.c_str(), content.c_str());
}

}
#endif

