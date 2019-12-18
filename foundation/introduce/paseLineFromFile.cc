#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

std::vector<std::vector<int>> readFile(std::string path) {
    std::vector<std::vector<int>> res;
    int x;
    char c;
    std::ifstream is(path);
    std::string line;
    while (std::getline(is, line)) {
        // 匹配
        std::vector<int> a;
        std::istringstream sis(line);
        while (sis >> x >> c) a.push_back(x);
        res.push_back(a);
    }
    return res;
}

void printVector(const std::vector<std::vector<int>> &a) {
    for (auto &c : a) {
        for (auto &p : c) std::cout << p << " ";
        std::cout << "\n";
    }
}

int main() {
    auto b = readFile("test.txt");
    printVector(b);
    return 0;
}
