#include <muduo/net/EventLoopThread.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include "ChatClient.h"

int main() {
    EventLoopThread thread;
    InetAddress addr(9090);
    ChatClient client(thread.startLoop(), addr);
    client.connect();
    std::string line;
    while (getline(std::cin, line)) {
        client.write(line);
    }
    client.disconnect();
    return 0;
}

