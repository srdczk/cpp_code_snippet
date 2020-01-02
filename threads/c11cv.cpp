#include <thread>
#include <mutex>
#include <condition_variable>
#include <unistd.h>
#include <list>
#include <iostream>
#include <chrono>

class Task {
public:
    Task(int id) { id_ = id;  }
    void task() {
        std::cout << "handle a task, ID:" << id_ << "\n";
    }
private:
        int id_;
};

std::mutex gMutex;
std::list<Task *> tasks;
std::condition_variable cond;


void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lck(gMutex);
        while (tasks.empty()) cond.wait(lck);
        std::cout << "NIAMSILE\n";
        auto task = tasks.front();
        tasks.pop_front();
        if (!task) continue;

        task->task();
        delete task;
        task = nullptr;

    }
}


void producer() {
    int id = 0;
    Task *task = nullptr;
    while (true) {
        task = new Task(id++);
        {
            std::lock_guard<std::mutex> guard(gMutex);
            tasks.push_back(task);
            std::cout << "PRODUCER id:" << id << "\n";
        }
        cond.notify_one();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    std::thread c1(consumer);
    std::thread c2(consumer);
    std::thread c3(consumer);
    std::thread c4(consumer);
    std::thread p(producer);
    c1.join();
    c2.join();
    c3.join();
    c4.join();
    p.join();
    return 0;
}
