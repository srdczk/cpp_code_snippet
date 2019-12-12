//
// Created by srdczk on 2019/12/12.
//

#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP

#include <cstring>

class chat_message {
public:
    enum { header_length = 4 };
    enum { max_length = 512 };
    chat_message() {
        std::memset(data_, '\0', header_length + max_length + 1);
    }

    int length() {
        return decodeHeader() + header_length;
    }

    void encodeHeader(int len) {
        if (len < 0) len = 0;
        if (len > max_length) len = max_length;
        for (int i = 0; i < header_length; ++i) {
            data_[i] = (len >> (8 * i)) & 0xff;
        }
    }

    int decodeHeader() {
        int len = 0;
        for (int i = 0; i < header_length; ++i) {
            len |= (data_[i] << (8 * i));
        }
        if (len < 0) len = 0;
        if (len > max_length) len = max_length;
        return len;
    }

    char *data() {
        return data_;
    }

    char *body() {
        return data_ + header_length;
    }

private:
    char data_[header_length + max_length + 1];
};

#endif //ASIO_CHAT_MESSAGE_HPP
