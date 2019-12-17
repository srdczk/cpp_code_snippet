//
// Created by srdczk on 2019/12/17.
//

#ifndef ECHO_H
#define ECHO_H

#include <muduo/net/TcpServer.h>
#include <iostream>

class EchoServer {
public:
    EchoServer(muduo::net::EventLoop *loop, muduo::net::InetAddress &listenAddress): loop_(loop), server_(loop, listenAddress, "EchoServer") {
        // 设置回调事件
        server_.setConnectionCallback([](const auto &conn) {
            // 直接通过 lambda 实现
            std::cout << "New Client:" << conn->peerAddress().toIpPort() << std::endl;
        });
        server_.setMessageCallback([](const auto &conn,
                                      auto *buffer,
                                      auto timeStamp) {
            // 回调大写
            muduo::string buf(buffer->retrieveAllAsString());
            for (auto &c : buf) {
                if (c >= 'a' && c <= 'z') c = c - 'a' + 'A';
            }
            conn->send(buf);
        });
    }

    void start() { server_.start(); }

private:
    // 事件池
    muduo::net::EventLoop *loop_;
    muduo::net::TcpServer server_;
};

#endif //MUDUO_ECHO_H
