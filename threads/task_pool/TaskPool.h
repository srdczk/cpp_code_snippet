//
// Created by srdczk on 20-1-2.
//

#ifndef TASKPOOL_H
#define TASKPOOL_H

#include <iostream>
#include <list>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "NotCopyable.h"

class Task {
public:
    virtual void task() { std::cout << "Do it\n"; }
    virtual ~Task() { std::cout << "Task destroy\n"; }
};

class TaskPool : public NotCopyable {
public:
    explicit TaskPool(int threadNum = 5);
    ~TaskPool();
    void stop();
    void addTask(Task *);
private:
    void threadFunc();
    std::list<std::shared_ptr<std::thread>> threads_;
    std::list<std::shared_ptr<Task>> taskList_;
    std::condition_variable cond_;
    std::mutex mutex_;
    bool running_;
};


#endif //TASK_POOL_TASKPOOL_H
