#include <iostream>
#include <boost/asio.hpp>

class printer {
public:
    printer(boost::asio::io_context &io)
        : timer_(io, boost::asio::chrono::seconds(1)), count_(0){
            timer_.async_wait([this](const auto &) { this->print(); });
        }

    void print() {
        if (count_ < 5) {
            std::cout << "Count:" << count_ << std::endl;
            ++count_;
            timer_.expires_at(timer_.expiry() + boost::asio::chrono::seconds(1));
            timer_.async_wait([this](const auto &) { this->print(); });
        }
    }

    ~printer() {
        std::cout << "Final count is "  << count_ << std::endl;
    }
private:
    int count_;
    boost::asio::steady_timer timer_;
};

int main() {
    boost::asio::io_context io;
    printer p(io);
    io.run();
    return 0;
}

