//
// Created by admin on 2020/1/3.
//

#include <cstring>
#include <iostream>
class Nima {
public:
    char x[16];
//    virtual void nima() = 0;
    Nima(const char *s) {
        std::memset(x, '\0', sizeof(x));
        std::strcpy(x, s);
    }
    bool operator<(const Nima &nima) {
        return std::strcmp(x, nima.x) < 0;
    }
};

int main() {
    std::cout << sizeof(Nima);
    return 0;
}

