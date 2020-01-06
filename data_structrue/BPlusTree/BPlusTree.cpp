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

// BPlusTree 的 删除
// 如果不存在, 返回 -1, 否则, 返回 0
int BPlusTree::del(const KeyType &key) {
    InternalNode parent;
    LeafNode leaf;
    off_t parentOffset = findIndex(key);
    readBlock(&parent, parentOffset);
    off_t leafOffset = findLeaf(parentOffset, key);
    readBlock(&leaf, leafOffset);

    if (!std::binary_search(begin(leaf), end(leaf), key)) return -1;
    // 最小 <--> order / 2;
    size_t minN = meta_.leafNodeNum == 1 ? 0 : meta_.order / 2;

    RecordType *toDelete = find(leaf, key);
    std::copy(toDelete + 1, end(leaf), toDelete);
    leaf.n--;

    // 结合或者向 兄弟借一个
    if (leaf.n < minN) {
        // 向兄弟 借 节点
        bool borrowed = false;
        if (leaf.pre) borrowed = borrowKey(false, leaf);
        if (!borrowed && leaf.next) borrowed = borrowKey(true, leaf);

        // 如果 没有借到, merge 并且向上删除
        if (!borrowed) {
            KeyType indexKey;
            IndexType *where = find(parent, key);
            if (where == end(parent) - 1) {
                LeafNode pre;
                readBlock(&pre, leaf.pre);
                indexKey = pre.children[0].key;
                mergeLeafs(&pre, &leaf);
                removeNode(&pre, &leaf);
                writeBlock(&pre, leaf.pre);
            } else {
                LeafNode next;
                readBlock(&next, leaf.next);
                indexKey = leaf.children[0].key;
                mergeLeafs(&leaf, &next);
                removeNode(&leaf, &next);
                writeBlock(&leaf, leaf.pre);
            }
            removeFromIndex(parentOffset, parent, indexKey);
        } else {
            writeBlock(&leaf, leafOffset);
        }
    } else {
        // 否则直接删除
        // 不用删除index, 因为索引的相对关系没有变
        writeBlock(&leaf, leafOffset);
    }

    return 0;
}

bool BPlusTree::borrowKey(bool fromRight, LeafNode &borrower) {
    off_t offSet = fromRight ? borrower.next : borrower.pre;
    LeafNode leaf;
    readBlock(&leaf, offSet);
    // 如果 leaf.n > meta_.order, 可以借出
    if (leaf.n > meta_.order / 2) {
        // change parent -> child
        RecordType *lend, *put;
        if (fromRight) {
            lend = begin(leaf);
            put = end(borrower);
            changeParentChild(borrower.parent, begin(borrower)->key
                    , leaf.children[1].key);
        } else {
            lend = end(leaf) - 1;
            put = begin(borrower);
            changeParentChild(leaf.parent, begin(leaf)->key
                    , lend->key);
        }
        // 存储起来
        std::copy_backward(put, end(borrower), end(borrower) + 1);
        *put = *lend;
        ++borrower.n;
        std::copy(lend + 1, end(leaf), lend);
        --leaf.n;
        writeBlock(&leaf, offSet);
        return true;
    }
    return false;

}

void BPlusTree::changeParentChild(off_t parent, const KeyType &o, const KeyType &n) {
    InternalNode node;
    readBlock(&node, parent);
    IndexType *w = find(node, o);
    w->key = n;
    writeBlock(&node, parent);
    if (w == node.children + node.n - 1) {
        changeParentChild(node.parent, o, n);
    }
}

void BPlusTree::removeFromIndex(off_t offset, InternalNode &node, const KeyType &key) {
    size_t minN = meta_.rootOffset == offset ? 1 : meta_.order / 2;
    KeyType indexKey = begin(node)->key;
    IndexType *todel = find(node, key);
    if (todel != end(node)) {
        (todel + 1)->child = todel->child;
        std::copy(todel + 1, end(node), todel);
    }
    --node.n;
    if (node.n == 1 && meta_.rootOffset == offset && meta_.internalNodeNum != 1) {
        // 如果 root 只剩下 一个指针了, 则 删除
        freeNode(&node, meta_.rootOffset);
        --meta_.height;
        // 不用 堆内存空间, 全部存储在磁盘上
        meta_.rootOffset = node.children[0].child;
        writeBlock(&meta_, OFFSET_META);
        return;
    }

    if (node.n < minN) {

        InternalNode parent;
        readBlock(&parent, node.parent);

        bool borrowed = false;
        // 只能够借同一个父亲的兄弟
        if (offset != begin(parent)->child) {
            borrowed = borrowKey(false, node, offset);
        }
        if (!borrowed && offset != (end(parent) - 1)->child) {
            borrowed = borrowKey(true, node, offset);
        }

        if (!borrowed) {

            if (offset == (end(parent) - 1)->child) {
                // 需要 merge 操作
                InternalNode pre;
                readBlock(&pre, node.pre);
                IndexType *where = find(parent, begin(pre)->key);
                resetIndexChildrenParent(begin(node), end(node), node.pre);
                mergeKeys(where, pre, node, true);
                writeBlock(&pre, node.pre);
            } else {
                InternalNode next;
                readBlock(&next, node.next);
                IndexType *where = find(parent, begin(node)->key);
                resetIndexChildrenParent(begin(next), end(next), offset);
                mergeKeys(where, node, next);
                writeBlock(&node, node.pre);
            }
            removeFromIndex(node.parent, parent, indexKey);
        }

    } else {
        // 直接重写
        writeBlock(&node, offset);
    }
}


bool BPlusTree::borrowKey(bool fromRight, InternalNode &borrower,
        off_t offset) {
    off_t lenderOff = fromRight ? borrower.next : borrower.pre;
    InternalNode lender;
    readBlock(&lender, lenderOff);

    if (lender.n > meta_.order / 2) {
        IndexType *lend, *put;
        InternalNode parent;

        // 需要重新设置指针
        if (fromRight) {
            lend = begin(lender);
            put = end(borrower);
            readBlock(&parent, borrower.parent);

            IndexType *where = std::lower_bound(begin(parent),
                    end(parent) - 1, (end(borrower) - 1)->key);
            where->key = lend->key;
            writeBlock(&parent, borrower.parent);
        } else {
            lend = end(lender) - 1;
            put = begin(borrower);

            readBlock(&parent, lender.parent);

            IndexType *where = find(parent, begin(lender)->key);

            where->key = (lend - 1)->key;
            writeBlock(&parent, lender.parent);
        }

        std::copy_backward(put, end(borrower), end(borrower) + 1);
        *put = *lend;
        ++borrower.n;

        resetIndexChildrenParent(lend, lend + 1, offset);

        std::copy(lend + 1, end(lender), lend);
        --lender.n;
        writeBlock(&lender, lenderOff);
        return true;
    }
    return false;
}

void BPlusTree::mergeLeafs(LeafNode *left,
        LeafNode *right) {
    std::copy(begin(*right), end(*right), end(*left));
    left->n += right->n;
}

void BPlusTree::mergeKeys(IndexType *where, InternalNode &node,
        InternalNode &next, bool change) {
    if (change) where->key = (end(next) - 1)->key;
    std::copy(begin(next), end(next), end(node));
    node.n += next.n;
    removeNode(&node, &next);
}
