#include "EventLoop.h"
#include "Server.h"

int main() {

    EventLoop loop;
    Server server;
    server.start();
    loop.loop();
    return 0;
}