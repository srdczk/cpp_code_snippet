//
// Created by admin on 2019/12/23.
//

#ifndef LOOP_SERVER_H
#define LOOP_SERVER_H

#include <memory>
#include <thread>
#include "EventLoop.h"

class Server {
public:
    Server() { std::cout << "Server Starting\n"; }
    ~Server() {
        if (thread_) {
            thread_->join();
            std::cout << "Thread stop\n";
        }
    }
    void start() {
        thread_ = std::make_shared<std::thread>([]() {
            while (EventLoop::isActive()) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << "Server running\n";
            }
        });
    }
private:
    std::shared_ptr<std::thread> thread_;
};

#endif //SERVER_H
