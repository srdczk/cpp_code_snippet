#include <stdio.h>
#include <unistd.h>

void func() {
    while (1) {
        printf("TEST1\n");
        sleep(1);
    }
}

int main() {
    int a[5] = {0};
    a[5] = (int)func;
    a[6] = (int)func;
    return 0;
}
