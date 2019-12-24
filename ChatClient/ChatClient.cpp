//
// Created by 86177 on 2019/12/24.
//

#include "ChatClient.h"

ChatClient::ChatClient(EventLoop *loop, InetAddress &addr): loop_(loop), client_(loop, addr, "ChatClient"), ptr_(nullptr) {
    client_.setConnectionCallback([this](const TcpConnectionPtr &ptr) {
        MutexLockGuard lock(mutex_);
        // 多线程端 调用 保护 ptr
        if (ptr->connected()) ptr_ = ptr;
        else ptr_.reset();
    });
    client_.setMessageCallback([this](const TcpConnectionPtr&,
                                      Buffer *buffer,
                                      Timestamp) {
        // 调用 printf 线程安全
        printf("<<< %s", buffer->retrieveAllAsString().data());
    });
}

void ChatClient::connect() { client_.connect(); }
void ChatClient::disconnect() { /*client_.disconnect();*/ }
void ChatClient::write(const StringPiece &piece) {
    // 发送
    MutexLockGuard lock(mutex_);
    if (ptr_) ptr_->send(piece);
}
