#include <iostream>
#include <boost/asio.hpp>
#include <thread>

class printer {
public:

    printer(boost::asio::io_context &io)
    : strand_(io),
    timer1_(io, boost::asio::chrono::seconds(1)),
    timer2_(io, boost::asio::chrono::seconds(1)),
    count_(0) { 
        timer1_.async_wait(strand_.wrap([this](const auto&) { print1(); }));
        timer2_.async_wait(strand_.wrap([this](const auto&) { print2(); }));
    }
    ~printer() {
        std::cout << "finish\n";
    }
    void print1() {
        if (count_ < 10) {
            std::cout << "Timer1:" << count_ << std::endl;
            ++count_;
            timer1_.expires_at(timer1_.expiry() + boost::asio::chrono::seconds(1));
            timer1_.async_wait(strand_.wrap([this](const auto &) { print1(); }));
        }
    }

    void print2() {
        if (count_ < 10) {
            std::cout << "Timer2:" << count_ << std::endl;
            ++count_;
            timer2_.expires_at(timer2_.expiry() + boost::asio::chrono::seconds(1));
            timer2_.async_wait(strand_.wrap([this](const auto &) { print2(); }));
        }
    }

private:
    // 多线程同步 <--> strand
    boost::asio::io_context::strand strand_;
    boost::asio::steady_timer timer1_;
    boost::asio::steady_timer timer2_;
    int count_;
};

int main() {
    boost::asio::io_context io;
    printer p(io);
    std::thread t([&io] { std::cout << "thread start\n"; io.run(); std::cout << "thread end\n"; });
    io.run();
    t.join();
    return 0;
}
