#include <cmath>
#include <iostream>
#include <thread>
#include <future>

int main() {
    double q = 3.0;
    std::promise<double> prms;
    auto ftr = prms.get_future();
    std::thread t([](auto &&prms, auto p) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        prms.set_value(sqrt(p));
    }, std::move(prms), q);
    auto status = ftr.wait_for(std::chrono::seconds(2));
    if (status == std::future_status::ready) std::cout << "Ready " << ftr.get() << std::endl;
    else std::cout << "Result unavailable" << std::endl;
    std::cout << ftr.get() << std::endl;
    t.join();
    return 0;
}