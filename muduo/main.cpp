#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include "TimeServer.h"

int main() {
    muduo::net::EventLoop loop;

    muduo::net::InetAddress listenAddr(9999);
;
    TimeServer server(&loop, listenAddr);
    server.start();
    loop.loop();
    return 0;
}