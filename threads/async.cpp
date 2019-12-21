#include <iostream>
#include <future>

int main() {
    double a = 4.0, b = 3.0;
    auto ftr = std::async([](auto num, auto p) {
        if (!p) throw std::runtime_error("divide 0");
        return num / p;
    }, a, b);
    try {
        std::cout << ftr.get() << std::endl;
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}