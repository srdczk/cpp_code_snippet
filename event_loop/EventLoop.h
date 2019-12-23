//
// Created by admin on 2019/12/23.
//

#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <unistd.h>
#include <signal.h>
#include <iostream>


class EventLoop {
public:
    EventLoop();
    static bool isActive();
    void loop();
private:
    static void sigHandler(int sig);
    void setIntHandler();
    static bool active_;
};



#endif //EVENT_LOOP_EVENTLOOP_H
