//
// Created by srdczk on 2019/12/12.
//
#include <iostream>
#include <string>
#include <deque>
#include <boost/asio.hpp>

#include "chat_message.hpp"

using boost::asio::ip::tcp;
using cmq = std::deque<chat_message>;

chat_message toChatMessage(std::string &s) {
    chat_message res;
    int len = s.length();
    if (len > chat_message::max_length) len = chat_message::max_length;
    res.encodeHeader(len);
    std::memcpy(res.body(), s.data(), len);
    return res;
}

class chat_client {
public:
    chat_client(boost::asio::io_context &io_context): io_context_(io_context), socket_(io_context) {
        do_connect();
    }
    void write(std::string &s) {
        bool pd = write_msgs_.empty();
        chat_message msg = toChatMessage(s);
        write_msgs_.push_back(msg);
        if (pd) do_write();
    }
private:
    void do_connect() {
        tcp::resolver resolver(io_context_);
        auto endpoints = resolver.resolve("127.0.0.1", "8989");
        boost::asio::async_connect(socket_, endpoints, [this](auto error, auto point) {
            if (!error) do_read_header();
        });
    }
    void do_read_header() {
        boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.data(), chat_message::header_length),
                [this](auto error, auto len) {
            if (!error) this->do_read_body();
            else socket_.close();
        });
    }
    void do_read_body() {
        boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.body(), read_msg_.decodeHeader()),
                                [this](auto error, auto len) {
                                    if (!error) {
                                        read_msg_.body()[read_msg_.decodeHeader()] = '\0';
                                        std::cout << read_msg_.body() << std::endl;
                                        do_read_header();
                                    } else socket_.close();
                                });
    }
    void do_write() {
        boost::asio::async_write(socket_, boost::asio::buffer(write_msgs_.front().data(), write_msgs_.front().length()),
                [this](auto error, auto len) {
            if (!error) {
                write_msgs_.pop_front();
                if (!write_msgs_.empty()) do_write();
            } else socket_.close();
        });
    }
    boost::asio::io_context &io_context_;
    tcp::socket socket_;
    chat_message read_msg_;
    cmq write_msgs_;
};

int main() {
    boost::asio::io_context io_context;
    chat_client client(io_context);
    std::thread t([&io_context]() { io_context.run(); });
    std::string line;
    while (getline(std::cin, line)) {
        client.write(line);
    }
    return 0;
}