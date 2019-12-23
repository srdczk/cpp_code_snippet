//
// Created by chengzekun on 2019/12/23.
//
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include <vector>

class Node {
public:
    Node(int x, int y, int g, int h) : x(x), y(y), g(g), h(h) {}
    [[nodiscard]] int fValue() const { return g + h; }
    [[nodiscard]] int gValue() const { return g; };
    [[nodiscard]] int xValue() const { return x; }
    [[nodiscard]] int yValue() const { return y; }
private:
    int x, y, g, h;
};

struct Cmp {
    bool operator()(const Node &a, const Node &b) {
        return a.fValue() >= b.fValue();
    }
};

std::priority_queue<Node, std::vector<Node>, Cmp> openList;

const int delta[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

enum class State { Empty, Closed, Path, Begin, End, Obstacle };

std::vector<State> readLine(const std::string& line) {
    std::istringstream is(line);
    std::vector<State> res;
    int p;
    char c;
    while (is >> p >> c && c == ',') {
        if (p) res.push_back(State::Obstacle);
        else res.push_back(State::Empty);
    }
    return res;
}

std::vector<std::vector<State>> readFile(const std::string &path) {
    std::fstream is(path);
    std::vector<std::vector<State>> res;
    if (is) {
        std::string line;
        while (getline(is, line)) res.push_back(readLine(line));
    }
    return res;
}

int calculateH(int x, int y, const std::vector<int> &end) {
    return (x > end[0] ? x - end[0] : end[0] - x) + (y > end[1] ? y - end[1] : end[1] - y);
}

bool isValid(std::vector<std::vector<State>> &map, int i, int j) {
    return i > -1 && i < map.size() && j > -1 && j < map[i].size() && map[i][j] == State::Empty;
}

void addNeighbor(std::vector<std::vector<State>> &map, const Node &node, const std::vector<int> &end) {
    for (auto const &i : delta) {
        int tx = node.xValue() + i[0], ty = node.yValue() + i[1];
        if (isValid(map, tx, ty)) {
            map[tx][ty] = State::Closed;
            int gValue = node.gValue() + 1, hValue = calculateH(tx, ty, end);
            openList.push(Node{tx, ty, gValue, hValue});
        }
    }
}

void aStarSearch(std::vector<std::vector<State>> &map, std::vector<int> &&begin, std::vector<int> &&end) {
    int x = begin[0], y = begin[1], g = 0, h = calculateH(begin[0], begin[1], end);
    openList.push({x, y, g, h});
    while (!openList.empty()) {
        Node node = openList.top();
        openList.pop();
        map[node.xValue()][node.yValue()] = State::Path;
        addNeighbor(map, node, end);
        if (node.xValue() == end[0] && node.yValue() == end[1]) {
            map[begin[0]][begin[1]] = State::Begin;
            map[end[0]][end[1]] = State::End;
            return;
        }
    }
}

std::string nodeString(State state) {
    switch(state) {
        case State::Obstacle: return "⛰️o   ";
        case State::Path: return "🚗p   ";
        case State::Begin: return "b🚦   ";
        case State::End: return "🏁e   ";
        default: return "0   ";
    }
}


void drawMap(const std::vector<std::vector<State>> &map) {
    for (auto &line : map) {
        for (auto &node : line) std::cout << nodeString(node);
        std::cout << "\n";
    }
}

int main() {
    auto map = readFile("map.txt");
    aStarSearch(map, {0, 0}, {4, 5});
    drawMap(map);
    return 0;
}
