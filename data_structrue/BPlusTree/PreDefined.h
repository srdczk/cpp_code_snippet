//
// Created by admin on 2020/1/3.
//

#ifndef PREDEFINED_H
#define PREDEFINED_H

#include <cassert>
#include <cstring>

// 子节点数目
#define BP_ORDER 20

// 值类型 -- int
using ValueType = int;

// 键 类型
class KeyType {
public:
    char k[16];
    explicit KeyType(const char *str = "") {
        std::memset(k, '\0', sizeof(k));
        std::strcpy(k, str);
    }

    explicit operator bool() const {
        return static_cast<bool>(strcmp(k, ""));
    }

};

inline int keyCmp(const KeyType &x, const KeyType &y) {
    int lenSub = static_cast<int>(std::strlen(x.k) - std::strlen(y.k));
    return lenSub ? lenSub : std::strcmp(x.k, y.k);
}

#define OPERATOR_KEYCMP(Type) \
    bool operator<(const KeyType &a, const Type &b) {\
        return keyCmp(a, b.key) < 0;\
    }\
    bool operator<(const Type &b, const KeyType &a) {\
        return keyCmp(b.key, a) < 0;\
    }\
    bool operator==(const KeyType &a, const Type &b) {\
        return !keyCmp(a, b.key);\
    }\
    bool operator==(const Type &b, const KeyType &a) {\
        return !keyCmp(a, b.key);\
    }

#endif //BPLUSTREE_PREDEFINED_H
