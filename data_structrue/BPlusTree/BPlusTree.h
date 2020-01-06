//
// Created by admin on 2020/1/3.
//

#ifndef BPLUSTREE_H
#define BPLUSTREE_H


#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include "PreDefined.h"

#define OFFSET_META 0
#define OFFSET_BLOCK OFFSET_META + sizeof(MetaType)
#define NO_CHILDREN_SIZE sizeof(LeafNode) - BP_ORDER * sizeof(RecordType)

// 元数据
struct MetaType {
    size_t order;      // B+ 树的阶数
    size_t valueSize;  // 值大小
    size_t keySize;    // 键 大小
    size_t internalNodeNum; // 内部节点的数量
    size_t leafNodeNum; // 叶子节点的数量
    size_t height;  // B+ 树的高度
    off_t slot;   // 新 块存储 偏移量
    off_t rootOffset; // 根节点存储 偏移量
    off_t leafOffset; // 起始叶节点偏移量
};
// 内部节点的索引
struct IndexType {
    KeyType key;
    // 子节点的偏移量
    off_t child;
};

// 内部节点
struct InternalNode {
    // 声明 IndexType *
    using ChildType = IndexType *;
    off_t parent; // 父节点偏移量
    off_t next;  // next 节点 偏移量
    off_t pre;
    // 子节点的数量
    size_t n;
    // 子节点的索引
    IndexType children[BP_ORDER];
};

struct RecordType {
    // 叶节点存储的记录
    KeyType key;
    ValueType val;
};

struct LeafNode {
    using ChildType = RecordType *;
    // 父节点偏移量
    off_t parent;
    off_t next;
    off_t pre;
    // record 数量
    size_t n;
    RecordType children[BP_ORDER];
};

class BPlusTree {
public:
    explicit BPlusTree(const char *, bool empty = false);
    // B+ 树 的操作 get (key), del, set, add
    int get(const KeyType &, ValueType *) const;
    int del(const KeyType &);
    int add(const KeyType &, ValueType &&);
    int set(const KeyType &, ValueType &&);
    MetaType getMeta() const { return meta_; }
private:
    char path_[512];
    MetaType meta_;
    mutable FILE *fp_;
    // 文件引用计数 <-->
    mutable int fpRefCount_;

private:
    void openFile(const char *mode = "rb+") const;
    void closeFile() const;

    // 向 磁盘 读节点
    template <typename T>
    int readBlock(T *block, off_t offset) const;
    int readBlock(void *block, off_t offset, size_t size) const;
    // 向 磁盘 写节点
    template <typename T>
    int writeBlock(T *block, off_t offset) const;
    int writeBlock(void *block, off_t offset, size_t size) const;

    // 创建新 节点
    template <typename T>
    void createNode(off_t offset, T *node, T *next);
    // 销毁节点
    template <typename T>
    void removeNode(T *pre, T *node);
    // 为节点分配/释放内存
    off_t alloc(size_t size);
    off_t alloc(LeafNode *leaf);
    off_t alloc(InternalNode *node);
    void freeNode(LeafNode *leaf, off_t offset);
    void freeNode(InternalNode *node, off_t offset);

    // 初始化存储结构
    void init();
    // 查找 叶节点 父节点 的偏移量
    off_t findIndex(const KeyType &key) const;
    // 返回叶节点 偏移量
    off_t findLeaf(const KeyType &key) const;
    off_t findLeaf(off_t parent, const KeyType &key) const;
    // 插入叶子节点, 无分裂
    void insertRecordNoSplit(LeafNode *node
            , const KeyType &key, const ValueType &val);

    // 分裂之后向 父节点插入
    void insertKeyToIndex(off_t offset, const KeyType& key,
                          off_t old, off_t after);
    // 不用分裂父节点的将分裂的新叶子节点的首个key插入父节点中
    void insertKeyToIndexNoSplit(InternalNode &node,
                                 const KeyType &key,
                                 off_t value);
    // 设置内部索引节点的父节点
    void resetIndexChildrenParent(IndexType* begin, IndexType* end, off_t parent);

    void removeFromIndex(off_t offset, InternalNode& node, const KeyType& key);

    void mergeLeafs(LeafNode* left, LeafNode* right);

    void mergeKeys(IndexType* where, InternalNode& node, InternalNode& next, bool change = false);

    bool borrowKey(bool fromRight, LeafNode& borrower);

    bool borrowKey(bool fromRight, InternalNode& borrower, off_t offset);

    void changeParentChild(off_t parent, const KeyType& o, const KeyType& n);

};
// 带模板 的直接 头文件 实现, 编译不过
template <typename T>
int BPlusTree::readBlock(T *block, off_t offset)const {
    return readBlock(block, offset, sizeof(block));
}
template <typename T>
int BPlusTree::writeBlock(T *block, off_t offset)const {
    return writeBlock(block, offset, sizeof(block));
}

template <typename T>
void BPlusTree::createNode(off_t offset, T *node, T *next) {
    next->parent = node->parent;
    next->pre = offset;
    next->next = node->next;
    node->next = alloc(next);
    if (next->next) {
        T block;
        readBlock(&block, next->next, NO_CHILDREN_SIZE);
        block->pre = node->next;
        writeBlock(&block, next->next, NO_CHILDREN_SIZE);
    }
    writeBlock(&meta_, OFFSET_META);
}
template <typename T>
void BPlusTree::removeNode(T *pre, T *node) {
    // removeNode <--> node 和 pre 的关系
    // node 的 offset pre->next
    freeNode(node, pre->next);
    pre->next = node->next;
    if (pre->next) {
        T node;
        // readBlock <--> read 不带索引的大小
        readBlock(&node, pre->next, NO_CHILDREN_SIZE);
        node.pre = pre;
        writeBlock(&node, pre->next, NO_CHILDREN_SIZE);
    }
    writeBlock(&meta_, OFFSET_META);
}
#endif //BPLUSTREE_BPLUSTREE_H
