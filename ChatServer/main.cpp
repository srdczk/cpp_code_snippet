#include "ChatServer.h"
#include <muduo/net/EventLoop.h>

int main() {
    EventLoop loop;
    InetAddress addr(9090);
    ChatServer server(&loop, addr);
    server.start();
    loop.loop();
    return 0;
}