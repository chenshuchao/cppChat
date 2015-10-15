# cppChat

## 简介

cppChat 实现了文字聊天室服务，目前实现功能，登录，注册，退出，文字聊天。

服务端(server): 实现一个带线程池的服务器，接收处理客户端请求。

客户端(client): 实现一个简单的终端交互界面，供用户输入。

数据存储: redis。

## 使用

### 依赖库

[boost] (version 1.59.0)

[rapidjson]


### 下载和编译

```
git clone https://github.com/chenshuchao/cppChat
cd cppChat
mkdir build
cd build
cmake ..
make
```

### 运行

启动 redis 服务


启动服务端

```
./server/server ../server/conf/server.conf
```


启动客户端

```
./client/client ../client/conf/client.conf
```


启动多个客户端后，注册登录后，输入 CHAT 进入聊天模式，客户端即可进入群聊模式。


## 其他

### todo

支持更多功能, 如多房间，单聊等。

丰富单元测试。


### 内部集成第三方库(无需自己安装)

[limonp]

C++ headers(hpp) with Python style.

[redispp]

C++ client for Redis

[limonp]:https://github.com/yanyiwu/limonp
[redispp]:https://github.com/brianwatling/redispp
[boost]:http://www.boost.org/
[rapidjson]:https://github.com/miloyip/rapidjson

