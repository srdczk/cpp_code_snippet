//
// Created by srdczk on 2019/12/30.
//

#ifndef RBTREE_H
#define RBTREE_H

#include <bits/shared_ptr.h>

template <typename K, typename V>
class RBTree {
public:
    RBTree();
    void add(K key, V val);
private:
    class Node;
    using NodePtr = std::shared_ptr<Node>;
    enum class Color { RED, BLACK };
    class Node {
        K key;
        V val;
        NodePtr left, right;
        Color color;
        Node(K k, V v): key(k), val(v) {
            left = nullptr;
            right = nullptr;
            color = Color::RED;
        }
    };

    NodePtr root;
    int size;


    void colorFlip(NodePtr node);
    NodePtr leftRotate(NodePtr node);
    NodePtr rightRotate(NodePtr node);

    bool isRed(NodePtr node);

    NodePtr add(NodePtr node, K key, V val);
};


#endif //RBTREE_RBTREE_H
