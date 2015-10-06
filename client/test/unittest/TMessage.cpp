#include <iostream>
#include <string>
#include "message.hpp"
using namespace std;
using namespace CppChat;

int main() {
    //string str1 = "{\"type\":bytree,\"account\":\"bytree\",\"password\":\"123\"}";
    cout << genLoginMsg("bytree", "123") << endl;
    string str2 = "{\"type\":1,\"status\":1000}";
    cout << isLoginSuccessfully(str2) << endl;
    string str3 = "{\"type\":1,\"status\":1001}";
    cout << isLoginSuccessfully(str3) << endl;
    string str4 = "{\"typ\":1,\"status\":1001}";
    cout << isLoginSuccessfully(str4) << endl;
} 

