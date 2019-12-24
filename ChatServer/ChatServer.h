//
// Created by 86177 on 2019/12/24.
//

#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <unordered_set>
#include <muduo/net/TcpServer.h>
#include "NotCopyable.h"

using namespace muduo::net;



class ChatServer : public NotCopyable {
public:
    using ConnectionSet = std::unordered_set<TcpConnectionPtr>;
    ChatServer(EventLoop *, InetAddress &);
    void start();
private:
    EventLoop *loop_;
    TcpServer server_;
    ConnectionSet set_;
};


#endif //CHATSERVER_CHATSERVER_H
