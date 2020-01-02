//
// Created by chengzekun on 20-1-2.
//

#include <functional>
#include "TaskPool.h"

TaskPool::TaskPool(int threadNum):running_(true) {
    if (threadNum < 0) threadNum = 5;
    for (int i = 0; i < threadNum; ++i) {
        threads_.push_back(std::make_shared<std::thread>(std::bind(&TaskPool::threadFunc, this)));
    }
}

void TaskPool::threadFunc() {
    while (true) {
        std::unique_lock<std::mutex> lck(mutex_);
        while (taskList_.empty()) {
            if (!running_) break;
            cond_.wait(lck);
        }
        if (taskList_.empty() && !running_) break;
        auto task = taskList_.front();
        taskList_.pop_front();
        if (!task) continue;
        task->task();
    }
}

void TaskPool::stop() {
    if (running_) {
        running_ = false;
        cond_.notify_all();
        for (auto &thread : threads_) {
            if (thread->joinable())
                thread->join();
        }
    }
}

TaskPool::~TaskPool() { stop(); }

void TaskPool::addTask(Task *task) {
    std::shared_ptr<Task> ptr(task);
    {
        // 自动释放锁
        std::lock_guard<std::mutex> guard(mutex_);
        taskList_.push_back(ptr);
        std::cout << "add a Task\n";
    }
    // 添加之后, 通知一个线程
    cond_.notify_one();
}
