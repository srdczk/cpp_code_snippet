//
// Created by srdczk on 2019/12/20.
//

#ifndef POINTER_H
#define POINTER_H

#include <cstdlib>
#include <list>
#include "PtrDetail.h"
// size 表示是否是数组
template <typename T, int size = 0>
class Pointer {
public:
private:
    // static <--> 所有的引用
    static std::list<PtrDetail<T>> refs;
    static bool first;
    T *addr;
    // 如果不是数组  size = 0;
    unsigned arraySize;
    // 返回 指定 Ptr -> 的 迭代器
    typename std::list<PtrDetail<T>>::iterator findPtrInfo(T *ptr);
public:
    // 无参构造器
    Pointer();
    explicit Pointer(T *);
    // 复制构造函数
    Pointer(const Pointer &);
    ~Pointer();
    // 整个 类 持有的 collect
    static bool collect();
    Pointer &operator=(T *);
    Pointer &operator=(const Pointer &);
    T &operator*();
    T *operator->();
    T &operator[](int);
    operator T*();
    static void shutdown();
};

#endif //POINTER_H
