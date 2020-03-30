#include <stdio.h>
#include <unistd.h>

// change esp to implement
void func() {
    while (1) {
        printf("TEST2\n");
        sleep(1);
    }
}


int main() {
    int stack[64] = {0};
    stack[63] = (int)func;
    int newStack = (int)(stack + 63);
    asm volatile ("mov %0, %%esp;"
                  "ret;" :: "r"(newStack));
    return 0;
}

