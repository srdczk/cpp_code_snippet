#include "Pointer.cpp"
#include "LeakTester.h"

int main() {
    Pointer<int> a(new int(19));
    a = new int(21);
    a = new int(18);
    Pointer<int> b(a);
    Pointer<int> c;
    c = a;
    return 0;
}