#ifndef _SKIPLIST

#define _SKIPLIST

#include <iostream>
#include <cmath>
#include <random>

using namespace std;

template <class K, class E>
struct skipNode
{                             // 跳表节点结构体
    pair<const K, E> element; // 节点元素
    skipNode<K, E> **next;    // 指针数组，next[i]表示i级链表指针

    skipNode(const pair<const K, E> &thePair, int size) : element(thePair)
    {                                      // 构造函数
        next = new skipNode<K, E> *[size]; // 按照size开辟指针数组
        for (int i = 0; i < size; i++)
            next[i] = nullptr; // 初始化每个指针为 nullptr
    }

    ~skipNode()
    {
        delete[] next; // 析构时释放指针数组
    }
};

template <class K, class E>
class skipList
{
private:
    float cutOff;               // 用来确定层数
    int levels;                 // 当前最大的非空链表
    int dSize;                  // 字典的数对个数
    int maxLevel;               // 允许的最大链表层数
    K tailKey;                  // 最大关键字
    skipNode<K, E> *headerNode; // 头节点指针
    skipNode<K, E> *tailNode;   // 尾节点指针
    skipNode<K, E> **last;      // last[i]表示i层的最后节点

public:
    // 构造函数
    skipList(K largeKey, int maxPairs, float prob);

    // 添加 getter 方法
    int getLevels() const { return levels; }
    skipNode<K, E> *getHeaderNode() const { return headerNode; }
    skipNode<K, E> *getTailNode() const { return tailNode; }
    int size() { return dSize; }

    // 跳表ADT
    pair<const K, E> *find(const K &theKey);       // 查找函数
    int level() const;                             // 级的分配
    skipNode<K, E> *search(const K &theKey) const; // 搜索并存储所遇最后一个节点指针
    void insert(const pair<const K, E> &thePair);  // 插入函数
    void erase(const K &theKey);                   // 按关键字删除
    void erase_min();                              // 删除关键字最小节点
    void erase_max();                              // 删除关键字最大节点
    void adjust();                                 // 重新调整跳表分层

    void printSkipListStructure();

public:
    // 迭代器类
    class iterator
    {
    private:
        skipNode<K, E> *current; // 当前节点

    public:
        iterator(skipNode<K, E> *startNode) : current(startNode) {}

        // 重载*运算符，返回当前节点的元素
        pair<const K, E> &operator*() { return current->element; }

        // 重载->运算符，返回当前节点的元素
        pair<const K, E> *operator->() { return &current->element; }

        // 重载++运算符，迭代到下一个节点
        iterator &operator++()
        {
            if (current != nullptr)
                current = current->next[0]; // 移动到下一个节点
            return *this;
        }

        // 重载!=运算符，比较两个迭代器是否不相等
        bool operator!=(const iterator &other) const
        {
            return current != other.current;
        }

        // 重载==运算符，比较两个迭代器是否相等
        bool operator==(const iterator &other) const
        {
            return current == other.current;
        }
    };

    // 返回指向跳表第一个元素的迭代器
    iterator begin()
    {
        return iterator(headerNode->next[0]);
    }

    // 返回指向跳表尾节点的迭代器
    iterator end()
    {
        return iterator(tailNode);
    }
};

// 时间复杂度：O(log maxPairs)
// 原因：我们要初始化尾结点
template <class K, class E>
skipList<K, E>::skipList(K largeKey, int maxPairs, float prob)
{
    // 计算随机数阈值，比这个大就说明可以提高一层
    cutOff = prob * RAND_MAX;
    maxLevel = (int)ceil(logf((float)maxPairs) / logf(1 / prob)) - 1; // 最大层数计算，推导如下
    /* 公式推导：
       - 基于概率模型：E(L(n)) = 1/(1-p) 的几何分布
       - 确保最大层数能满足 maxPairs 元素的概率分布
       - 例如当prob=0.5，maxPairs=1000时：
         log(1000)/log(2) ≈ 9.96 → 9.96-1=8.96 → ceil后maxLevel=9
    */
    if (maxLevel < 0)
        maxLevel = 0; // 确保maxLevel不小于0
    levels = 0;
    dSize = 0;
    tailKey = largeKey; // 令尾结点大小为最大

    pair<K, E> tailPair(tailKey, E());
    headerNode = new skipNode<K, E>(tailPair, maxLevel + 1);
    tailNode = new skipNode<K, E>(tailPair, maxLevel + 1); // 修正tailNode大小
    last = new skipNode<K, E> *[maxLevel + 1];             // 初始化辅助数组，可以方便的用于插入删除

    for (int i = 0; i <= maxLevel; i++)
        headerNode->next[i] = tailNode;
    /* 可视化初始状态：
        Level 3: head -> tail
        Level 2: head -> tail
        Level 1: head -> tail
        Level 0: head -> tail
    */
}

// 时间复杂度：O(log n)平均
// 原因：跳表结构给出二分
template <class K, class E>
pair<const K, E> *skipList<K, E>::find(const K &theKey)
{ // 就是利用跳表结构二分查找
    skipNode<K, E> *beforeNode = headerNode;
    for (int i = levels; i >= 0; i--)
    { // 对于每层，确定当前元素在哪两个节点之间，然后向下
        while (beforeNode->next[i] != tailNode && beforeNode->next[i]->element.first < theKey)
            beforeNode = beforeNode->next[i]; // 还没找到继续往前
    }
    return (beforeNode->next[0] != tailNode && beforeNode->next[0]->element.first == theKey) // 到最后一层了，看看找到的对不对
               ? &beforeNode->next[0]->element                                               // 找到了
               : nullptr;                                                                    // 没找到
}

// 时间复杂度：O(1)
template <class K, class E>
inline int skipList<K, E>::level() const
{
    int lev = 0;
    while (rand() <= cutOff)
        lev++; // 利用rand来计算当前节点的层数
    return (lev <= maxLevel) ? lev : maxLevel;
}

// 时间复杂度：O(log n)
// 原因：同find
template <class K, class E>
skipNode<K, E> *skipList<K, E>::search(const K &theKey) const
{ // 找一个合适位置，与find不同，这个找到与没找到都会返回位置，可以用于插入和删除
    skipNode<K, E> *beforeNode = headerNode;
    for (int i = levels; i >= 0; i--)
    {
        while (beforeNode->next[i] != tailNode && beforeNode->next[i]->element.first < theKey)
            beforeNode = beforeNode->next[i];
        last[i] = beforeNode;
    }
    return beforeNode->next[0];
}

// 时间复杂度：O(log n)
// 原因：搜索+层级调整
template <class K, class E>
void skipList<K, E>::insert(const pair<const K, E> &thePair)
{
    if (thePair.first >= tailKey)
    { // 报错
        cout << "关键字太大，超出范围" << endl;
        return;
    }

    skipNode<K, E> *theNode = search(thePair.first); // 找到插入的位置（或者已经有人了）
    if (theNode != nullptr && theNode->element.first == thePair.first)
    { // 如果有人了，覆盖
        theNode->element.second = thePair.second;
        return;
    }
    // 否则，插入新节点
    int theLevel = level(); // 计算层级初始化节点
    if (theLevel > levels)
    { // 当前节点的高度高于目前最高高度
        theLevel = ++levels;
        last[theLevel] = headerNode;
    }

    skipNode<K, E> *newNode = new skipNode<K, E>(thePair, theLevel + 1);
    for (int i = 0; i <= theLevel; i++)
    { // 插入节点就要连接指针
        newNode->next[i] = last[i]->next[i];
        last[i]->next[i] = newNode;
    }

    for (int i = 0; i <= theLevel; i++)
        last[i] = newNode;

    dSize++;
}

// 时间复杂度：O(log n)
// 原因：搜索+指针维护
template <class K, class E>
void skipList<K, E>::erase(const K &theKey)
{
    if (theKey >= tailKey)
        return;

    skipNode<K, E> *theNode = search(theKey); // 还是先找
    if (theNode == nullptr || theNode->element.first != theKey)
        return; // 没找到

    for (int i = 0; i <= levels && last[i]->next[i] == theNode; i++)
        last[i]->next[i] = theNode->next[i]; // 找到了，改变指针

    while (levels > 0 && headerNode->next[levels] == tailNode)
        levels--; // 如果是该层唯一节点，层数--

    delete theNode;
    dSize--;
}

// 时间复杂度：O(log n)
// 原因：指针维护
template <class K, class E>
void skipList<K, E>::erase_min()
{ // 第一个节点就是最小节点，用相同的方法删除第一个就是了
    if (dSize == 0)
        return;

    skipNode<K, E> *theNode = headerNode->next[0];
    for (int i = 0; i <= levels && headerNode->next[i] == theNode; i++)
        headerNode->next[i] = theNode->next[i];

    while (levels > 0 && headerNode->next[levels] == tailNode)
        levels--;
    delete theNode;
    dSize--;
}

// 时间复杂度：O(n)
// 原因：需要遍历底层链表
template <class K, class E>
void skipList<K, E>::erase_max()
{ // 最后一个节点就是最小节点，用相同的方法删除它就是了
    if (dSize == 0)
        return;

    skipNode<K, E> *current = headerNode;
    while (current->next[0] != tailNode)
        current = current->next[0];

    if (current != headerNode)
        erase(current->element.first);
}

// 时间复杂度：O(n)
// 原因：需要遍历所有节点进行重构
template <class K, class E>
void skipList<K, E>::adjust()
{ // 把当前跳表调整为严格跳表
    // 收集第0层的所有节点（按顺序）
    vector<skipNode<K, E> *> nodes;
    skipNode<K, E> *current = headerNode->next[0];
    while (current != tailNode)
    {
        nodes.push_back(current);
        current = current->next[0];
    }
    int numNodes = nodes.size();

    // 计算每个节点的新层级和新的最大层数
    int newMaxLevel = 0;
    vector<int> nodeLevels(numNodes);
    for (int i = 0; i < numNodes; ++i)
    {
        int pos = i + 1; // 节点位置从1开始
        int level = 0;
        // 计算层级：二进制表示中末尾0的个数
        while ((pos & 1) == 0)
        {
            level++;
            pos >>= 1;
        }
        nodeLevels[i] = level;
        if (level > newMaxLevel)
        {
            newMaxLevel = level;
        }
    }

    // 调整头节点和尾节点的层级（如果必要）
    if (newMaxLevel > maxLevel)
    {
        // 扩展头节点和尾节点的next数组
        skipNode<K, E> **newHeaderNext = new skipNode<K, E> *[newMaxLevel + 1];
        skipNode<K, E> **newTailNext = new skipNode<K, E> *[newMaxLevel + 1];

        // 复制旧层的数据，新层初始指向尾节点
        for (int i = 0; i <= maxLevel; ++i)
        {
            newHeaderNext[i] = headerNode->next[i];
            newTailNext[i] = tailNode->next[i];
        }
        for (int i = maxLevel + 1; i <= newMaxLevel; ++i)
        {
            newHeaderNext[i] = tailNode;
            newTailNext[i] = nullptr; // 尾节点的next通常为nullptr
        }

        delete[] headerNode->next;
        delete[] tailNode->next;
        headerNode->next = newHeaderNext;
        tailNode->next = newTailNext;
        maxLevel = newMaxLevel;
    }
    levels = newMaxLevel;

    // 初始化每层的最后节点指针（初始为头节点）
    vector<skipNode<K, E> *> lastInLayer(newMaxLevel + 1, headerNode);

    // 遍历每个节点，重新链接各层
    for (int i = 0; i < numNodes; ++i)
    {
        skipNode<K, E> *node = nodes[i];
        int level = nodeLevels[i];

        // 检查并调整节点的next数组大小
        int requiredSize = level + 1;
        if (node->next == nullptr || (node != headerNode && node != tailNode && sizeof(node->next) / sizeof(node->next[0]) < requiredSize))
        {
            delete[] node->next;
            node->next = new skipNode<K, E> *[requiredSize];
            // 初始化新分配的指针
            for (int q = 0; q < requiredSize; ++q)
            {
                node->next[q] = tailNode;
            }
        }

        // 更新各层链接
        for (int q = 0; q <= level; ++q)
        {
            lastInLayer[q]->next[q] = node;
            node->next[q] = tailNode;
            lastInLayer[q] = node;
        }
    }

    // 确保头节点各层的末尾正确指向尾节点
    for (int q = 0; q <= newMaxLevel; ++q)
    {
        lastInLayer[q]->next[q] = tailNode;
    }
}

// 时间复杂度：O(n)
// 原因：要遍历
template <class K, class E>
void skipList<K, E>::printSkipListStructure()
{ // 用来调试的输出函数
    // 从最高层开始，依次打印每一层
    cout << "跳表结构：" << endl;

    // 打印每一层的节点
    for (int i = levels; i >= 0; i--) // 从最高层到最低层
    {
        skipNode<K, E> *current = headerNode->next[i];
        cout << "层 " << i << ": ";

        // 遍历当前层的所有节点
        while (current != tailNode)
        {
            cout << current->element.first << " ";
            current = current->next[i];
        }

        cout << endl;
    }
}

#endif