#include <cmath>
#include <iostream>
#include <memory>


class Node;

using NodePtr = std::shared_ptr<Node>;

class Node {
public:
    int val;
    NodePtr left, right;
    Node(int v): val(v), left(nullptr), right(nullptr) {}
};

int height(NodePtr node) {
    if (!node) return 0;
    return std::max(height(node->left), height(node->right)) + 1;
}

NodePtr LL(NodePtr node) {
    auto left = node->left;
    node->left = left->right;
    left->right = node;
    return left;
}

NodePtr RR(NodePtr node) {
    auto right = node->right;
    node->right = right->left;
    right->left = node;
    return right;
}

NodePtr LR(NodePtr node) {
    node->left = RR(node->left);
    return LL(node);
}

NodePtr RL(NodePtr node) {
    node->right = LL(node->right);
    return RR(node);
}

NodePtr insert(NodePtr node, int x) {
    if (!node) return std::make_shared<Node>(x);
    if (node->val > x) {
        node->left = insert(node->left, x);
        if (height(node->left) - height(node->right) == 2) {
            if (node->left->val > x) node = LL(node);
            else node = LR(node);
        }
    } else {
        node->right = insert(node->right, x);
        if (height(node->right) - height(node->left) == 2) {
            if (node->right->val > x) node = RL(node);
            else node = RR(node);
        }
    }
    return node;
}

int main() {
    int n;
    std::cin >> n;
    NodePtr node;
    while (n--) {
        int x;
        std::cin >> x;
        node = insert(node, x);
    }
    std::cout << node->val << "\n";
    return 0;
}
