#include <boost/asio.hpp>
#include <iostream>
#include <ctime>
#include <string>

using boost::asio::ip::tcp;

std::string make_daytime_string() {
    time_t now = time(nullptr);
    return ctime(&now);
}

int main() {
    try {
        boost::asio::io_context io_context;
        // endpoint <--> 端口号
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 13));
        while (true) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            std::string msg = make_daytime_string();
            boost::system::error_code ignore_code;
            boost::asio::write(socket, boost::asio::buffer(msg), ignore_code);
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
