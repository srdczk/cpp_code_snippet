#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <list>

enum { max_size = 5 };

std::mutex mutex;
std::condition_variable condA, condB;

std::list<int> queue;

int main() {
    std::thread producer([]() {
        // 生产者
        for (int i = 0; i < 20; ++i) {
            std::unique_lock<std::mutex> lck(mutex);
            // 构造函数自动上锁
            while (queue.size() == max_size) condA.wait(lck);
            queue.push_back(i);
            std::cout << "Produce: " << i << " Size:" << queue.size() << "\n";
            condB.notify_all();
            lck.unlock();
        }
    });
    std::thread consumer([]() {
        while (true) {
            if (queue.size() < 0) break;
            std::unique_lock<std::mutex> lck(mutex);
            while (queue.empty()) condB.wait(lck);
            auto k = queue.front();
            queue.pop_front();
            std::cout << "Consumer: " << k << " Size: " << queue.size() << "\n";
            condA.notify_all();
            lck.unlock();
        }
    });
    producer.join();
    consumer.join();
    return 0;
}
