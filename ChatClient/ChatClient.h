//
// Created by 86177 on 2019/12/24.
//

#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <muduo/net/TcpClient.h>

using namespace muduo::net;
using namespace muduo;
class ChatClient {
public:
    ChatClient(EventLoop *loop, InetAddress &addr);
    void write(const StringPiece &);
    void connect();
    void disconnect();
private:
    EventLoop *loop_;
    TcpClient client_;
    MutexLock mutex_;
    TcpConnectionPtr ptr_;
};


#endif //CHATCLIENT_CHATCLIENT_H
