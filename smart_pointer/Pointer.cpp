//
// Created by srdczk on 2019/12/20.
//
#include "Pointer.h"

// 静态变量 , 类外初始化
template <typename T, int size>
std::list<PtrDetail<T>> Pointer<T, size>::refs;

template <typename T, int size>
bool Pointer<T, size>::first = true;

template <typename T, int size>
typename std::list<PtrDetail<T>>::iterator Pointer<T, size>::findPtrInfo(T *ptr) {
    auto it = refs.begin();
    while (it != refs.end()) {
        if (it->memPtr == ptr) break;
        ++it;
    }
    return it;
}

template <typename T, int size>
Pointer<T, size>::Pointer(T *ptr) {
    if (first) atexit(shutdown);
    first = false;
    addr = ptr;
    arraySize = size;
    auto it = findPtrInfo(ptr);
    if (it == refs.end()) refs.push_back(PtrDetail<T>(ptr, size));
    else ++it->refCount;
}

template <typename T, int size>
Pointer<T, size>::Pointer(): Pointer(nullptr) {}

template <typename T, int size>
bool Pointer<T, size>::collect() {
    auto it = refs.begin();
    bool res = false;
    while (it != refs.end()) {
        if (!it->refCount) {
            if (it->memPtr) {
                if (it->arraySize) {
                    delete[] it->memPtr;
                    it->memPtr = nullptr;
                } else {
                    delete it->memPtr;
                    it->memPtr = nullptr;
                }
            }
            it = refs.erase(it);
            res = true;
        } else ++it;
    }
    return res;
}

template <typename T, int size>
T &Pointer<T, size>::operator*() { return *addr; }

template <typename T, int size>
T* Pointer<T, size>::operator->() { return addr; }

template <typename T, int size>
T &Pointer<T, size>::operator[](int i) { return addr[i]; }

template <typename T, int size>
Pointer<T, size>::operator T*() { return addr; }

template <typename T, int size>
void Pointer<T, size>::shutdown() {
    auto it = refs.begin();
    while (it != refs.end()) {
        it->refCount = 0;
        ++it;
    }
    collect();
}

template <typename T, int size>
Pointer<T, size> &Pointer<T, size>::operator=(const Pointer &pointer) {
    // 已经有了一个对象
    auto it = findPtrInfo(addr);
    if (!--it->refCount) collect();
    addr = pointer.addr;
    arraySize = pointer.arraySize;
    it = findPtrInfo(addr);
    ++it->refCount;
    return *this;
}

template <typename T, int size>
Pointer<T, size> &Pointer<T, size>::operator=(T *ptr) {
    auto it = findPtrInfo(addr);
    if (--it->refCount) collect();
    it = findPtrInfo(ptr);
    if (it == refs.end()) {
        refs.push_back(PtrDetail<T>(ptr));
    } else ++it->refCount;
    addr = ptr;
    arraySize = 0;
    return *this;
}

template <typename T, int size>
Pointer<T, size>::Pointer(const Pointer &pointer) {
    auto it = findPtrInfo(pointer.addr);
    ++it->refCount;
    addr = pointer.addr;
    arraySize = pointer.arraySize;
}

template <typename T, int size>
Pointer<T, size>::~Pointer() {
    auto it = findPtrInfo(addr);
    if (!--it->refCount) collect();
}