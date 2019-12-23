//
// Created by admin on 2019/12/23.
//

#include "EventLoop.h"

bool EventLoop::active_ = true;

bool EventLoop::isActive() { return active_; }

void EventLoop::sigHandler(int sig) {
    std::cout << "End sig:" << sig <<"\n";
    active_ = false;
}

void EventLoop::setIntHandler() {
    struct sigaction act;
    act.sa_handler = sigHandler;
    act.sa_flags = SA_INTERRUPT;
    sigaction(SIGINT, &act, nullptr);
}

void EventLoop::loop() { sleep(-1); }

EventLoop::EventLoop() { setIntHandler(); }