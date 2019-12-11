#include <memory>
#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this<session> {
public:
    session(tcp::socket socket): socket_(std::move(socket)) {}
    void start() { do_read(); }
private:
    void do_read() {
        auto self = shared_from_this();
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
                [self = std::move(self)](auto error, auto len) {
            if (!error) self->do_write(len);
        });
    }
    void do_write(std::size_t len) {
        auto self = shared_from_this();
        boost::asio::async_write(socket_, boost::asio::buffer(data_, len),
                [self = std::move(self)](auto error, auto len) {
            if (!error) self->do_read();
        });

    }
    tcp::socket socket_;
    // 数据缓存
    enum { max_length = 1024 };
    char data_[max_length];
};

class server {
public:
    server(boost::asio::io_context &io_context): acceptor_(io_context, tcp::endpoint(tcp::v4(), 8080)), socket_(io_context) {
        do_accept();
    }
private:
    void do_accept() {
        acceptor_.async_accept(socket_, [this](auto error) {
            if (!error) std::make_shared<session>(std::move(this->socket_))->start();
            this->do_accept();
        });
    }
    tcp::acceptor acceptor_;
    tcp::socket socket_;
};

int main() {
    try {
        boost::asio::io_context io_context;
        server s(io_context);
        io_context.run();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}