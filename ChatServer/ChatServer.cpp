//
// Created by 86177 on 2019/12/24.
//

#include "ChatServer.h"

ChatServer::ChatServer(EventLoop *loop, InetAddress &addr):loop_(loop), server_(loop, addr, "ChatServer") {
    server_.setConnectionCallback([this](const TcpConnectionPtr &ptr) {
        if (ptr->connected()) this->set_.insert(ptr);
        else this->set_.erase(ptr);
    });
    server_.setMessageCallback([this](const auto &,
                                      auto *buffer,
                                      auto) {
        auto msg = buffer->retrieveAllAsString();
        for (auto &ptr : set_) {
            ptr->send(msg);
        }
    });
}

void ChatServer::start() { server_.start(); }