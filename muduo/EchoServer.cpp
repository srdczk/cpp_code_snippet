//
// Created by admin on 2019/12/23.
//

#include "EchoServer.h"


EchoServer::EchoServer(muduo::net::EventLoop *loop, muduo::net::InetAddress &listenAddr): loop_(loop),
server_(loop, listenAddr, "EchoServer") {

//    const TcpConnectionPtr&,
//    Buffer*,
//            Timestamp
    server_.setMessageCallback([](const auto &ptr, auto *buffer, auto) {
        muduo::string msg = buffer->retrieveAllAsString();
        for (auto &c : msg) {
            if (c >= 'a' && c <= 'z') c = 'A' + c - 'a';
        }
        ptr->send(msg);
    });

}

void EchoServer::start() { server_.start(); }