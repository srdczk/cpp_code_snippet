#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

enum class State { kEmpty, kObstacle, kClosed, kPath, kStart, kFinish };

int delta[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

int heuristic(int a, int b, int c, int d) {
    return (a > c ? a - c : c - a) + (b > d ? b - d : d - b);
}

void cellSort(std::vector<std::vector<int>> *openList) {
    std::sort(openList->begin(), openList->end(), [](const auto &x, const auto &y) { return x[2] + x[3] > y[2] + y[3]; });
}

bool isValidCell(int x, int y, std::vector<std::vector<State>> &grid) {
    return grid[x][y] == State::kEmpty;
}

void addToOpenList(std::vector<int> &&node, std::vector<std::vector<int>> &openList, std::vector<std::vector<State>> &grid) {
    openList.push_back(node);
    grid[node[0]][node[1]] = State::kClosed;
}

void expandNeiborgh(std::vector<int> &cur, std::vector<std::vector<int>> &openList, std::vector<int> &goal, std::vector<std::vector<State>> &grid) {
    int cx = cur[0], cy = cur[1];
    for (int i = 0; i < 4; ++i) {
        int tx = cx + delta[i][0], ty = cy + delta[i][1];
        if (tx > -1 && tx < grid.size() && ty > -1 && ty < grid[0].size() && isValidCell(tx, ty, grid)) {
            addToOpenList({tx, ty, cur[2] + 1, heuristic(tx, ty, goal[0], goal[1])}, openList, grid);
        }
    }

}

std::vector<std::vector<State>> aStarSearch(std::vector<std::vector<State>> grid, std::vector<int> init, std::vector<int> goal) {
    std::vector<std::vector<int>> openList;
    openList.push_back({init[0], init[1], 0, heuristic(init[0], init[1], goal[0], goal[1])});
    while (!openList.empty()) {
        cellSort(&openList);
        auto cur = openList.back();
        openList.pop_back();
        // 扩展邻居
        expandNeiborgh(cur, openList, goal, grid);
        grid[cur[0]][cur[1]] = State::kPath;
        if (cur[0] == goal[0] && cur[1] == goal[1]) {
            grid[goal[0]][goal[1]] = State::kFinish;
            grid[init[0]][init[1]] = State::kStart;
            return grid;
        }
    }
    return grid;
}

std::vector<State> parseLine(std::string line) {
    std::vector<State> res;
    std::istringstream is(line);
    int x;
    char c;
    while (is >> x >> c && c == ',') {
        if (!x) res.push_back(State::kEmpty);
        else res.push_back(State::kObstacle);
    }
    return res;
}

std::vector<std::vector<State>> parseFile(std::string path) {
    std::ifstream is(path);
    std::vector<std::vector<State>> res;
    if (is) {
        std::string line;
        while (getline(is, line)) res.push_back(parseLine(line));
    }
    return res;
}

std::string cellString(State state) {
    switch(state) {
        case State::kObstacle: return "⛰️   ";
        case State::kPath: return "🚗   ";
        case State::kStart: return "🚦   ";
        case State::kFinish: return "🏁   ";
        default: return "0   ";
    }
}

void print(const std::vector<std::vector<State>> &a) {
    for (auto &p : a) {
        for (auto &c : p) std::cout << cellString(c);
        std::cout << "\n";
    }
}

// astar Search
int main() {
    auto grid = parseFile("1.board");
    print(grid);
    std::cout << "\n";
    auto solution = aStarSearch(grid, {0, 0}, {4, 5});
    print(solution);
    return 0;
}
