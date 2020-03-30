#include <stdio.h>
#include <unistd.h>

int task[3] = {0, 0, 0};
int cur = 0;

void switch_to(int i);

void func1() {
    while (1) {
        printf("FUN1\n");
        sleep(1);
        switch_to(2);
    }
}

void func2() {
    while (1) {
        printf("FUN2\n");
        sleep(1);
        switch_to(1);
    }
}

void start(int i) {
    if (i == 1) func1();
    else if(i == 2) func2();
}


int main() {
    int stack1[1024] = {0};
    int stack2[1024] = {0};
    task[1] = (int)(stack1 + 1021);
    task[2] = (int)(stack2 + 1021);
    stack1[1021] = (int)start;
    stack1[1022] = 100;
    stack1[1023] = 1;

    stack2[1021] = (int)start;
    stack2[1022] = 100;
    stack2[1023] = 2;


    switch_to(1);
}
