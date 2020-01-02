#include <thread>
#include <chrono>
#include <iostream>

thread_local int g_data = 1;

void thread_func1() {
    while (true) {
        ++g_data;
    }
}

void thread_func2() {
    while (true) {
        std::cout << g_data << " Thread:" << std::this_thread::get_id() << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    std::thread t1(&thread_func1);
    std::thread t2(&thread_func2);
    t1.join();
    t2.join();
    return 0;
}
