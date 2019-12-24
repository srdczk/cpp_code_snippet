//
// Created by 86177 on 2019/12/24.
//

#ifndef NOTCOPYABLE_H
#define NOTCOPYABLE_H


class NotCopyable {
public:
    NotCopyable() = default;
    NotCopyable(const NotCopyable &) = delete;
    NotCopyable(NotCopyable &&) = delete;
    NotCopyable &operator=(NotCopyable &&) = delete;
    NotCopyable &operator=(const NotCopyable &) = delete;
    ~NotCopyable() = default;
};


#endif //CHATSERVER_NOTCOPYABLE_H
