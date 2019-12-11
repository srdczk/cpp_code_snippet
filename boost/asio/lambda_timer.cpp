#include <iostream>
#include <boost/asio.hpp>

// lambda 表达式表示 timer 的函数

void print(const boost::system::error_code &e,
           boost::asio::steady_timer *t, int *count) {
    if (*count < 5) {
        std::cout << "Count:" << *count << std::endl;
        ++(*count);
        t->expires_at(t->expiry() + boost::asio::chrono::seconds(1));
        // t, count 都是指针类型
        t->async_wait([t, count](const auto &error) { print(error, t, count); });
    }
}
int main() {
    boost::asio::io_context io;
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(1));
    int count = 0;
    t.async_wait([&t, &count](const auto &error) { print(error, &t, &count); });
    io.run();
    return 0;
}
