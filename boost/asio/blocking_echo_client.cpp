#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

enum { max_length = 1024 };

int main(int ac, char **av) {
    try {
        if (ac != 3) {
            std::cerr << "Usage <host> <port>" << std::endl;
            return 1;
        }
        boost::asio::io_context io_context;

        tcp::socket socket(io_context);
        tcp::resolver resolver(io_context);
        boost::asio::connect(socket, resolver.resolve(av[1], av[2]));

        while (true) {
            std::cout << "Enter message:";
            char buff[max_length];
            std::cin.getline(buff, max_length);
            if (!std::strcmp(buff, "q\n")) break;
            boost::asio::write(socket, boost::asio::buffer(buff, std::strlen(buff)));
            char reply[max_length];
            std::memset(reply, '\0', max_length);
            std::size_t len = boost::asio::read(socket, boost::asio::buffer(reply, std::strlen(buff)));
            std::cout << "Reply is: " << reply << std::endl;
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}