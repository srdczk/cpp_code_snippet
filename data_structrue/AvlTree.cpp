//
// Created by srdczk on 2019/12/17.
//

#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include <memory>

class Node {
public:
    int val;
    std::shared_ptr<Node> left, right;
    Node(int v): val(v), left(nullptr), right(nullptr) {}
};

class AvlTree {
public:
    AvlTree(): root(nullptr) {}

    int rootVal() const { return root->val; }

    void insert(int x) {
        root = insert(root, x);
    }

private:
    int height(std::shared_ptr<Node> node) {
        if (!node) return 0;
        return std::max(height(node->left), height(node->right)) + 1;
    }
    // LL, RR, LR, RL
    std::shared_ptr<Node> LL(std::shared_ptr<Node> &node) {
        auto left = node->left;
        auto right = left->right;
        left->right = node;
        node->left = right;
        return left;
    }
    std::shared_ptr<Node> RR(std::shared_ptr<Node> &node) {
        auto right = node->right;
        auto left = right->left;
        right->left = node;
        node->right = left;
        return right;
    }
    std::shared_ptr<Node> LR(std::shared_ptr<Node> &node) {
        node->left = RR(node->left);
        return LL(node);
    }
    std::shared_ptr<Node> RL(std::shared_ptr<Node> &node) {
        node->right = LL(node->right);
        return RR(node);
    }
    std::shared_ptr<Node> insert(std::shared_ptr<Node> &node, int x) {
        if (!node) {
            return std::make_shared<Node>(x);
        }
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
    std::shared_ptr<Node> root;
};

#endif //AVLTREE_HPP
