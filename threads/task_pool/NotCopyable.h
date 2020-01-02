//
// Created by srdczk on 20-1-2.
//

#ifndef NOTCOPYABLE_H
#define NOTCOPYABLE_H

class NotCopyable {
public:
    NotCopyable() = default;
    ~NotCopyable() = default;
    NotCopyable(const NotCopyable &) = delete;
    NotCopyable(NotCopyable &&) = delete;
    NotCopyable &operator=(const NotCopyable &) = delete;
    NotCopyable &operator=(NotCopyable &&) = delete;
};

#endif //TASK_POOL_NOTCOPYABLE_H
