//
// Created by admin on 2019/12/23.
//
#include <ctime>
#include "TimeServer.h"

TimeServer::TimeServer(muduo::net::EventLoop *loop, muduo::net::InetAddress &listenAddr):loop_(loop),
server_(loop, listenAddr, "TimeServer") {

    server_.setConnectionCallback([](const muduo::net::TcpConnectionPtr &ptr) {
        auto now = time(nullptr);
        muduo::string timeMsg = ctime(&now);
        if (ptr->connected()) {
            ptr->send(timeMsg);
            ptr->shutdown();
        }
    });

}


void TimeServer::start() { server_.start(); }
