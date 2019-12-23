//
// Created by admin on 2019/12/23.
//

#ifndef TIMESERVER_H
#define TIMESERVER_H

#include <muduo/net/TcpServer.h>

class TimeServer {
public:
    TimeServer(muduo::net::EventLoop *, muduo::net::InetAddress &);

    void start();
private:
    muduo::net::EventLoop *loop_;
    muduo::net::TcpServer server_;
};


#endif //MUDUO_TIMESERVER_H
