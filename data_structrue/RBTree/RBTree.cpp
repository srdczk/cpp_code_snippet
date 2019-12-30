//
// Created by srdczk on 2019/12/30.
//

#include "RBTree.h"

template <typename K, typename V>
RBTree<K, V>::RBTree(): root(nullptr), size(0) {}

template <typename K, typename V>
void RBTree<K, V>::add(K key, V val) {
    root = add(root, key, val);
    root->color = RBTree::Color::BLACK;
}

template <typename K, typename V>
typename RBTree<K, V>::NodePtr RBTree<K, V>::add(RBTree::NodePtr node, K key, V val) {
    if (!root) {
        size++;
        return std::make_shared<Node>(key, val);
    }
    if (key < node->key) node->left = add(node->left, key, val);
    else if (key > node->key) node->right = add(node->right, key, val);
    else node->val = val;

    if (!isRed(node->left) && isRed(node->right)) leftRotate(node);

    if (isRed(node->left) && isRed(node->left->left)) rightRotate(node);

    if (isRed(node->left) && isRed(node->right)) colorFlip(node);

    return node;

}

template <typename K, typename V>
bool RBTree<K, V>::isRed(RBTree::NodePtr node) {
    return node && node->color == Color::RED;
}

template <typename K, typename V>
typename RBTree<K, V>::NodePtr RBTree<K, V>::leftRotate(RBTree::NodePtr node) {
    NodePtr x = node->right;
    node->right = x->left;
    x->left = node;
    x->color = node->color;
    node->color = Color::RED;
    return x;
}

template <typename K, typename V>
typename RBTree<K, V>::NodePtr RBTree<K, V>::rightRotate(RBTree::NodePtr node) {
    NodePtr x = node->left;
    node->left = x->right;
    x->right = node;
    x->color = node->color;
    node->color = Color::RED;
    return x;
}

template <typename K, typename V>
void RBTree<K, V>::colorFlip(RBTree::NodePtr node) {
    node->color = Color::BLACK;
    node->left->color = Color::RED;
    node->right->color = Color::RED;
}