#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

int main(int ac, char **av) {
    try {
        if (ac != 2) {
            std::cerr << "Usage: client <host>" << std::endl;
            return 1;
        }
        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
		// 服务号或者端口号
        tcp::resolver::results_type endpoints =
            resolver.resolve(av[1], "daytime");
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        while (true) {
            boost::array<char, 128> buff;
            boost::system::error_code error;
            
            size_t len = socket.read_some(boost::asio::buffer(buff), error);

            if (error == boost::asio::error::eof) break;
            else if (error) throw boost::system::system_error(error);
            
            std::cout.write(buff.data(), len);
        }

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
