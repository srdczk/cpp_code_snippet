#include <iostream>
#include <thread>
#include <future>
#include <exception>

int main() {
    int p = 0;
    std::promise<int> prms;
    auto ftr = prms.get_future();
    std::thread t([](auto &&prms, auto q) {
        try {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            if (!q) throw std::runtime_error("Exception from thread: Division by zero!");
            prms.set_value(q);
        } catch (...) {
            prms.set_exception(std::current_exception());
        }
        }, std::move(prms), p);
    try {
        std::cout << ftr.get() << std::endl;
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
    }
    t.join();
    return 0;
}