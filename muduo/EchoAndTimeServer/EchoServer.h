//
// Created by admin on 2019/12/23.
//

#ifndef ECHOSERVER_H
#define ECHOSERVER_H


#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>

class EchoServer {
public:
    EchoServer(muduo::net::EventLoop *, muduo::net::InetAddress &);
    void start();
private:
    muduo::net::EventLoop *loop_;
    muduo::net::TcpServer server_;
};


#endif //MUDUO_ECHOSERVER_H
