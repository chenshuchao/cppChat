#ifndef CPPCHAT_CONSTANT_HPP
#define CPPCHAT_CONSTANT_HPP

namespace cppchat {

const int TYPE_LOGIN = 1;
const int TYPE_LOGOUT = 2;
const int TYPE_REGISTER = 3;
const int TYPE_CHAT = 4;
const int TYPE_CHAT_RESP = 5;

const int SUCCESS = 1000;
const int FAIL = 2000;

const size_t RECV_BUFFER_SIZE = 16 * 1024;
}

#endif

