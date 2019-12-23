//
// Created by chengzekun on 2019/12/23.
//

#include <ctime>
#include "TimeServer.h"

TimeServer::TimeServer(muduo::net::EventLoop *loop, muduo::net::InetAddress &listenAddr): loop_(loop), server_(loop, listenAddr, "TimeServer") {
    // 连接 之后 直接断开
    server_.setConnectionCallback([](const auto &ptr) {
        auto now = time(nullptr);
        muduo::string timeString = ctime(&now);
        if (ptr->connected()) {
            ptr->send(timeString);
            ptr->shutdown();
        }
    });
}

void TimeServer::start() { server_.start(); }