#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

enum class State { KEmpty, KObstacle };

std::vector<State> parseLine(std::string &line) {
    std::vector<State> res;
    std::istringstream is(line);
    int x;
    char c;
    while (is >> x >> c && c == ',') {
        if (x) res.push_back(State::KObstacle);
        else res.push_back(State::KEmpty);
    }
    return res;
}

std::vector<std::vector<State>> parseFile(std::string path) {
    std::vector<std::vector<State>> res;
    std::ifstream is(path);
    if (is) {
        std::string line;
        while (std::getline(is, line)) res.push_back(parseLine(line));
    }
    return res;
}

std::string parseState(State state) {
    if (state == State::KObstacle) return "⛰️   ";
    else return "0   ";
}

void printVector(const std::vector<std::vector<State>> &a) {
    for (auto &c : a) {
        for (auto p : c) std::cout << parseState(p);
        std::cout << "\n";
    }
}

int main() {
    auto a = parseFile("test.txt");
    printVector(a);
    return 0;
}