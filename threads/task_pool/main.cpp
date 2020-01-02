//
// Created by chengzekun on 20-1-2.
//

#include "TaskPool.h"

class Nima : public Task {
public:
    void task() override {
        int cnt = 0;
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            if (cnt++ == 10) break;
            std::cout << "NIMA\n";
        }
    }
};

class Pp : public Task {
public:
    void task() override { std::cout << "PPP\n"; }
};

int main() {
    TaskPool pool(3);
    pool.addTask(new Nima);
    pool.addTask(new Pp);
    return 0;
}