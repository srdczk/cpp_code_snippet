#include <ctime>

#include <iostream>
#include <string>
#include <memory>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_string() {
    return "NIMASIE\n";
}

std::string make_daytime_string() {
    auto now = time(nullptr);
    return ctime(&now);
}

// 内部回调函数使用自己的智能指针
class tcp_connection : public std::enable_shared_from_this<tcp_connection> {
public:
    using ptr = std::shared_ptr<tcp_connection>;

    static ptr create(boost::asio::io_context &io_context) {
        return ptr(new tcp_connection(io_context));
    } 
    tcp::socket &socket() { return socket_;}

    void start() {
        msg_ = make_string();

        auto self = shared_from_this();

        boost::asio::async_write(socket_, boost::asio::buffer(msg_), [self = std::move(self)](auto x, auto y) {
                                 self->handle_write(x, y);
                                 });

    }

private:

    tcp_connection(boost::asio::io_context &io_context): socket_(io_context) {}

    void handle_write(const boost::system::error_code &,
                      size_t) {

    }

    tcp::socket socket_;
    std::string msg_;
};
class tcp_server {
public:
    tcp_server(boost::asio::io_context &io_context)
        :io_context_(io_context), acceptor_(io_context_, tcp::endpoint(tcp::v4(), 8080)) {
            start_accept();
        }
private:

    void start_accept() {
        auto new_connection = tcp_connection::create(io_context_);
        acceptor_.async_accept(new_connection->socket(), [this, new_connection](auto error) {
                               this->handle_accept(new_connection, error);
                               });
    }
    void handle_accept(tcp_connection::ptr p, const boost::system::error_code &error) {
        if (!error) p->start();
        start_accept();
    }
    boost::asio::io_context &io_context_;
    tcp::acceptor acceptor_;
};

int main() {
    try {
        boost::asio::io_context io_context;
        tcp_server server(io_context);
        io_context.run();
    } catch(std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
