//
// Created by srdczk on 2019/12/17.
//

#ifndef TIME_H
#define TIME_H

#include <ctime>
#include <muduo/net/TcpServer.h>

class TimeServer {
public:
    TimeServer(muduo::net::EventLoop *loop, muduo::net::InetAddress &listenAddress): loop_(loop), server_(loop, listenAddress, "TimeServer") {
        // 建立连接之后直接发送时间并且断开
        server_.setConnectionCallback([](const auto &conn) {
            time_t now = std::time(nullptr);
            // 转换成大端
            // 直接发送
            muduo::string msg = ctime(&now);
            conn->send(msg);
        });
    }
    void start() { server_.start(); }
private:
    muduo::net::EventLoop *loop_;
    muduo::net::TcpServer server_;
};

#endif //MUDUO_TIME_H
