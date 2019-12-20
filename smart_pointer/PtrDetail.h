//
// Created by srdczk on 2019/12/20.
//

#ifndef PTRDETAIL_H
#define PTRDETAIL_H

template <typename T>
class PtrDetail {
public:
    // 引用计数
    unsigned refCount;
    // 内存
    T *memPtr;
    unsigned arraySize;
    // 如果不是数组 size = 0
    PtrDetail(T *ptr, unsigned size = 0) {
        refCount = 1;
        memPtr = ptr;
        arraySize = size;
    }
};

template <typename T>
bool operator==(const PtrDetail<T> &x, const PtrDetail<T> &y) {
    return x.arraySize == y.arraySize && x.memPtr == y.memPtr;
}

#endif //PTRDETAIL_H
