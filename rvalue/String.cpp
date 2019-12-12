#include <iostream>
#include <cstring>

class String {
public:
    String(): length(0), buff(new char[1] {'\0'}) {}
    String(const char *s): length(std::strlen(s)), buff(new char[std::strlen(s) + 1]) {
        std::memcpy(buff, s, std::strlen(s) + 1);
    }
    String(const String &s): length(s.length), buff(new char[s.length + 1]) {
        std::memcpy(buff, s.buff, length + 1);
    }
    String(String &&s): length(s.length), buff(s.buff) {
        s.buff = nullptr;
    }
    String &operator=(const String &s) {
        // 重新申请空间
        if (this != &s) {
            delete[] buff;
            length = s.length;
            buff = new char[length + 1];
            std::memcpy(buff, s.buff, length + 1);
        }
        return *this;
    }
    // 右值引用 用于 move
    String &operator=(String &&s) {
        if (this != &s) {
            delete[] buff;
            length = s.length;
            buff = s.buff;
            s.buff = nullptr;
        }
        return *this;
    }
    // 为了方便const & 类型调用
    bool operator==(const String &s) const { return !Compare(s); }
    bool operator!=(const String &s) const { return Compare(s); }
    bool operator<(const String &s) const { return Compare(s) < 0; }
    bool operator>(const String &s) const { return Compare(s) > 0; }
    bool operator<=(const String &s) const { return Compare(s) <= 0; }
    bool operator>=(const String &s) const { return Compare(s) >= 0; }
private:
    int Compare(const String &s) const {
        return std::strcmp(buff, s.buff);
    }
    int length;
    char *buff;
};