//
// Created by srdczk on 2019/12/17.
//
#include <muduo/net/EventLoop.h>
#include "Echo.h"
#include "Time.h"

int main() {
    muduo::net::EventLoop loop;
    muduo::net::InetAddress echoAddress(9090), timeAddress(9797);
    EchoServer echoServer(&loop, echoAddress);
    TimeServer timeServer(&loop, timeAddress);
    timeServer.start();
    echoServer.start();
    loop.loop();
    return 0;
}

