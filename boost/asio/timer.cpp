#include <boost/asio.hpp>
#include <iostream>

int main() {

    // io_context 
    boost::asio::io_context io;

    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));

    // 同步计时器 <--> 阻塞等待

    t.wait();

    std::cout << "NIMAISLE" << std::endl;

    return 0;
}
