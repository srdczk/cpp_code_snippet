#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

// bind 绑定函数参数

void print(const boost::system::error_code &e,
           boost::asio::steady_timer *t, int *count) {
    if (*count < 5) {
        std::cout << "Count:" << *count << std::endl;
        ++(*count);

//        用bind 绑定参数
        t->expires_at(t->expiry() + boost::asio::chrono::seconds(1));
        t->async_wait(boost::bind(print, boost::asio::placeholders::error, t, count));
    }
}

int main() {
    boost::asio::io_context io;
    int count = 0;
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(1));
    t.async_wait(boost::bind(print, boost::asio::placeholders::error, &t, &count));
    io.run();
    std::cout << count << std::endl;
    return 0;
}
