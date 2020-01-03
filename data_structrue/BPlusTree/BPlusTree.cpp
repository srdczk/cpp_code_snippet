//
// Created by admin on 2020/1/3.
//

#include <cstdlib>
#include <list>
#include <algorithm>
#include "BPlusTree.h"

// 声明 重载 比较类型
OPERATOR_KEYCMP(IndexType)
OPERATOR_KEYCMP(RecordType)

template <typename T>
inline typename T::ChildType begin(T &node) {
    return node.children;
}

template <typename T>
inline typename T::ChildType end(T &node) {
    return node.children + node.n;
}

inline IndexType *find(InternalNode &node, const KeyType &key) {
    if (key) {
        // 大于 key 的第一个位置
        return std::upper_bound(begin(node), end(node), key);
    }
    // b+ 树 内部节点 最后一个存储 空字符
    if (node.n > 1) return node.children + node.n - 2;
    return begin(node);
}
inline RecordType *find(LeafNode &node, const KeyType &key) {
    // >= val 的 第一个位置
    return std::lower_bound(begin(node), end(node), key);
}

BPlusTree::BPlusTree(const char *path, bool empty): fp_(nullptr), fpRefCount_(0) {
    std::memset(path_, '\0', sizeof(path_));
    std::strcpy(path_, path);
    if (!empty) {
        if (readBlock(&meta_, OFFSET_META)) empty = true;
    }

    if (empty) {
        // 重置 file 指针
        openFile("w+");
        // initTree file 不存在
        init();
        closeFile();
    }
}

int BPlusTree::get(const KeyType &key, ValueType *val) const {
    LeafNode node;
    // find Leaf
    readBlock(&node, findLeaf(key));
    RecordType *record = find(node, key);
    if (record != node.children + node.n) {
        *val = record->val;
        return keyCmp(record->key, key);
    } else {
        return -1;
    }
}

int BPlusTree::add(const KeyType &key, ValueType &&val) {
    // 找到插入的 father Index
    off_t parent = findIndex(key);
    off_t offset = findLeaf(parent, key);
    LeafNode leaf;
    // 读取 leaf  节点
    readBlock(&leaf, offset);
    // keep 一个 hash 的索引
    // 判断 key 是否已经存在, 直接返回,
    if (std::binary_search(begin(leaf), end(leaf), key)) return 1;

    // 如果已经要 分裂了
    if (leaf.n == meta_.order) {
        LeafNode newLeaf;
        // newLeaf -> next;
        // offse 里边 读取 node
        createNode(offset, &leaf, &newLeaf);

        // leaf.n / 2 --> split
        size_t cutPoint = leaf.n / 2;
        // 判断放在 左边还是 右边
        // 5 / 2 = 2
        bool isRight = keyCmp(key, leaf.children[cutPoint].key) > 0;
        if (isRight) ++cutPoint;
        // 复制, 直接在新分裂 的 节点上插入
        std::copy(leaf.children + cutPoint, leaf.children + leaf.n, newLeaf.children);
        newLeaf.n = leaf.n - cutPoint;
        leaf.n = cutPoint;

        if (isRight) insertRecordNoSplit(&newLeaf, key, val);
        else insertRecordNoSplit(&leaf, key, val);

        // 重新 写入 磁盘
        writeBlock(&leaf, offset);
        writeBlock(&newLeaf, leaf.next);
        // 向 父节点插入
        insertKeyToIndex(parent, newLeaf.children[0].key,
                offset, leaf.next);
    } else {
        insertRecordNoSplit(&leaf, key, val);
        writeBlock(&leaf, offset);
    }
    return 0;
}
// google Test <--->
void BPlusTree::insertRecordNoSplit(LeafNode *node
        , const KeyType &key, const ValueType &val) {
    // 插入 第一个 > 他 的地方
    RecordType *insert = std::upper_bound(begin(*node), end(*node), key);
    // copy <-->
    std::copy_backward(insert, end(*node), end(*node) + 1);
    insert->key = key;
    insert->val = val;
    ++node->n;
}

void BPlusTree::insertKeyToIndex(off_t offset, const KeyType &key, off_t old, off_t after) {
    // (root : parent == 0)
    // 需要新建 root node. root node 有两个 儿子节点 <> old and after
    if (!offset) {
        InternalNode node;
        node.next = node.pre = node.parent = 0;
        meta_.rootOffset = alloc(&node);
        meta_.height++;

        node.n = 2;
        node.children[0].key = key;
        node.children[0].child = old;
        node.children[1].child = after;

        // 重新写入
        writeBlock(&meta_, OFFSET_META);
        writeBlock(&node, meta_.rootOffset);
        // reset begin() <--> end()
        resetIndexChildrenParent(begin(node), end(node)
                , meta_.rootOffset);
        return;
    }
    InternalNode node;
    readBlock(&node, offset);
    assert(node.n <= BP_ORDER);
    // node.n == BP_ORDER split
    if (node.n == BP_ORDER) {
        InternalNode newNode;
        createNode(offset, &node, &newNode);
        size_t cutPoint = (node.n - 1) / 2;
        bool isRight = keyCmp(key, node.children[cutPoint].key) > 0;
        if (isRight) ++cutPoint;
        if (isRight && keyCmp(key, node.children[cutPoint].key) < 0) --cutPoint;
        // midKey -> toInsertUp
        KeyType mid = node.children[cutPoint].key;
        std::copy(begin(node) + cutPoint + 1, end(node), begin(newNode));
        newNode.n = node.n - cutPoint - 1;
        node.n = cutPoint + 1;
        if (isRight) insertKeyToIndexNoSplit(newNode, key, after);
        else insertKeyToIndexNoSplit(node, key, after);
        writeBlock(&node, offset);
        writeBlock(&newNode, node.next);
        resetIndexChildrenParent(begin(newNode), end(newNode), node.next);
        insertKeyToIndex(node.parent, mid, offset, node.next);
    } else {
        insertKeyToIndexNoSplit(node, key, after);
        writeBlock(&node, offset);
    }
}

void BPlusTree::insertKeyToIndexNoSplit(InternalNode &node
        , const KeyType &key, off_t value) {
    IndexType *insert = std::upper_bound(begin(node), end(node) - 1, key);
    std::copy_backward(insert, end(node), end(node) + 1);
    insert->key = key;
    insert->child = (insert + 1)->child;
    (insert + 1)->child = value;
    node.n++;
}

off_t BPlusTree::findLeaf(const KeyType &key) const {
    return findLeaf(findIndex(key), key);
}

off_t BPlusTree::findLeaf(off_t parent, const KeyType &key) const {
    InternalNode node;
    readBlock(&node, parent);
    auto *p = std::upper_bound(begin(node), end(node) - 1, key);
    return p->child;
}

off_t BPlusTree::findIndex(const KeyType &key) const {
    // 一开始 的 root
    off_t pos = meta_.rootOffset;
    int height = static_cast<int>(meta_.height);
    while (height > 1) {
        InternalNode node;
        readBlock(&node, pos);
        auto *p = std::upper_bound(begin(node), end(node) - 1, key);
        pos = p->child;
        --height;
    }
    return pos;
}

void BPlusTree::resetIndexChildrenParent(IndexType *begin
        , IndexType *end, off_t parent) {
    InternalNode node;
    while (begin != end) {
        readBlock(&node, begin->child);
        node.parent = parent;
        // 叶节点 和 节点 的大小 均相同
        writeBlock(&node, begin->child, NO_CHILDREN_SIZE);
        ++begin;
    }
}
