//
// Created by srdczk on 2019/12/12.
//
#include <iostream>
#include <memory>
#include <deque>
#include <unordered_set>

#include <boost/asio.hpp>

#include "chat_message.hpp"

class chat_part;

using boost::asio::ip::tcp;
using cmq = std::deque<chat_message>;
using ptr = std::shared_ptr<chat_part>;

class chat_part {
public:
    virtual ~chat_part() {}
    virtual void deliver(const chat_message &) = 0;
};

class chat_room {
public:
    void join(ptr p) {
        ptrs_.insert(p);
        for (auto &msg : msgs_) {
            p->deliver(msg);
        }
    }
    void leave(ptr p) {
        ptrs_.erase(p);
    }
    void deliver(const chat_message &msg) {
        msgs_.push_back(msg);
        while (msgs_.size() > max_size) msgs_.pop_front();
        for (auto &ptr : ptrs_) ptr->deliver(msg);
    }
private:
    enum { max_size = 100 };
    std::unordered_set<ptr> ptrs_;
    cmq msgs_;
};

class chat_session: public chat_part, public std::enable_shared_from_this<chat_session> {
public:
    chat_session(tcp::socket socket, chat_room &room): socket_(std::move(socket)), room_(room) {}
    void start() {
        room_.join(shared_from_this());
        do_read_header();
    }
    void deliver(const chat_message &msg) override {
        bool pd = write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (pd) do_write();
    }
private:
    void do_read_header() {
        auto self = shared_from_this();
        boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.data(), chat_message::header_length),
                [self = std::move(self)](auto error, auto len) {
            if (!error) self->do_read_body();
            else self->room_.leave(self);
        });
    }
    void do_read_body() {
        auto self = shared_from_this();
        boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.body(), read_msg_.decodeHeader()),
                                [self = std::move(self)](auto error, auto len) {
                                    if (!error) {
                                        self->room_.deliver(self->read_msg_);
                                        self->do_read_header();
                                    }
                                    else self->room_.leave(self);
                                });
    }
    void do_write() {
        auto self = shared_from_this();
        boost::asio::async_write(socket_, boost::asio::buffer(write_msgs_.front().data(), write_msgs_.front().length()),
                [self = std::move(self)](auto error, auto len) {
            if (!error) {
                self->write_msgs_.pop_front();
                if (!self->write_msgs_.empty()) self->do_write();
            } else self->room_.leave(self);
        });
    }
    tcp::socket socket_;
    chat_room &room_;
    chat_message read_msg_;
    cmq write_msgs_;
};

class chat_server {
public:
    chat_server(boost::asio::io_context &io_context): socket_(io_context), acceptor_(io_context, tcp::endpoint(tcp::v4(), 8989)) {
        do_accept();
    }
private:
    void do_accept() {
        acceptor_.async_accept(socket_, [this](auto error) {
            if (!error) {
                std::make_shared<chat_session>(std::move(this->socket_), this->room_)->start();
            }
            do_accept();
        });
    }
    tcp::socket socket_;
    tcp::acceptor acceptor_;
    chat_room room_;
};

int main() {
    try {
        boost::asio::io_context io_context;
        chat_server server(io_context);
        io_context.run();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}