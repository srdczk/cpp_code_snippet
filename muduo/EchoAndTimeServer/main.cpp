#include "TimeServer.h"
#include "EchoServer.h"
#include <muduo/net/EventLoop.h>


int main() {

    muduo::net::EventLoop loop;
    muduo::net::InetAddress listenAddr(8989);
//    TimeServer server(&loop, listenAddr);
    EchoServer server(&loop, listenAddr);
    server.start();
    loop.loop();

    return 0;
}