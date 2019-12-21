#include <iostream>
#include <thread>
#include <future>

int main() {
    std::string message = "Message";
    std::promise<std::string> prms;
    std::future<std::string> ftr = prms.get_future();
    std::thread t([](auto &&prms, auto msg) {
        std::this_thread::sleep_for(std::chrono::seconds(6));
        prms.set_value(msg + " has been modified");
    }, std::move(prms), message);
    std::cout << message << std::endl;
    std::cout << ftr.get() << std::endl;
    t.join();
    return 0;
}