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

inline string genResponse(int type, int status, const char *msg) {
    return string_format("{\"type\":\"%s\",\"status\":\"%s\",\"content\":\"%s\"}", to_string(type).c_str(), to_string(status).c_str(), msg);
}
inline string genChatMsg(int type, string &sender, string &content) {
    return string_format("{\"type\":\"%s\",\"sender\":\"%s\",\"content\":\"%s\"}", to_string(type).c_str(), sender.c_str(), content.c_str());
}

}

#endif

