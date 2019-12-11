#include <boost/asio.hpp>
#include <iostream>

void callback_a(const boost::system::error_code &) {
    std::cout << "!" << std::endl;
}

void callback_b(const boost::system::error_code &) {
    std::cout << "@" << std::endl;
}

int main() {
    // 异步等待 <--> 非阻塞 回调
    // io_context
    boost::asio::io_context io;

    boost::asio::steady_timer a(io, boost::asio::chrono::seconds(2));
    boost::asio::steady_timer b(io, boost::asio::chrono::seconds(5));

    a.async_wait(&callback_a);
    b.async_wait(&callback_b);
    io.run();
    return 0;
} 
