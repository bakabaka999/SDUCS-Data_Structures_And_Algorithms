#ifndef _CHAIN_H
#define _CHAIN_H

#include <bits\stdc++.h>

using namespace std;

// 输入值错误异常定义
class illegalParameterValue
{
public:
    illegalParameterValue(string theMessage = "Illegal parameter value")
    {
        message = theMessage;
    }
    void outputMessage() { cout << message << endl; }

private:
    string message;
};

// 索引错误异常定义
class illegalIndex
{
public:
    illegalIndex(string theMessage = "Illegal index")
    {
        message = theMessage;
    }
    void outputMessage() { cout << message << endl; }

private:
    string message;
};

// 结点定义
template <class T>
struct chainNode
{
    // 数据成员
    T element;
    chainNode<T> *next;

    // 方法(构造函数)
    chainNode() {}
    chainNode(const T &e) : element(e) {}
    chainNode(const T &e, chainNode<T> *p) : element(e), next(p) {}
};

// 链表定义
template <class T>
class chain
{
protected:
    void checkIndex(int theIndex) const; // 检查索引正确性
    chainNode<T> *firstNode;             // 头节点地址
    chainNode<T> *lastNode;              // 尾结点地址
    int listSize;                        // 线性表元素个数
public:
    // 构造函数与析构函数
    chain(int initialCapacity = 10);
    chain(const chain<T> &theList);
    ~chain();

    // 线性表方法
    bool empty() const { return listSize == 0; }    // 线性表判空
    int size() const { return listSize; }           // 获取线性表大小
    T &get(int theIndex) const;                     // 按索引获取元素
    int indexOf(const T &theElement) const;         // 获得元素索引
    void erase(int theIndex);                       // 按索引删除元素
    void insert(int theIndex, const T &theElement); // 按索引插入元素
    void reverse();                                 // 逆置链表
    void push_back(const T &theElement);            // 链表末尾插入
    void output(ostream &out) const;                // 向输出流输出内容
    void set(int theIndex, T &theElement);          // 替换元素

    // 迭代器嵌套类
    class iterator;
    iterator begin() { return iterator(firstNode); } // 返回头部迭代器
    iterator end() { return iterator(NULL); }        // 返回尾部迭代器（为空）

    // 链表类迭代器的定义
    class iterator
    {
    public:
        // 用C++的typedef语句实现单向迭代器
        typedef forward_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef T *pointer;
        typedef T &reference;

        // 构造函数
        iterator(chainNode<T> *theNode = NULL) : node(theNode) {}

        // 解引用操作符
        T &operator*() const { return node->element; }
        T &operator->() const { return &node->element; }

        // 迭代器加法操作
        iterator &operator++()
        {
            node = node->next;
            return *this;
        }
        iterator &operator++(int)
        {
            iterator old = *this;
            node = node->next;
            return old;
        }

        // 迭代器相等检验
        bool operator!=(const iterator right) const { return node != right.node; }
        bool operator==(const iterator right) const { return node == right.node; }

    protected:
        chainNode<T> *node;
    };
};

template <class T>
void chain<T>::checkIndex(int theIndex) const
{
    if (theIndex < 0 || theIndex >= listSize)
    {
        ostringstream s;
        s << "index = " << theIndex << " size = " << listSize;
        throw illegalIndex(s.str());
    }
}

template <class T>
chain<T>::chain(int initialCapacity)
{
    if (initialCapacity < 1)
    {
        ostringstream s;
        s << "Initial capacity = " << initialCapacity << " Must be > 0";
        throw illegalParameterValue(s.str());
    }
    firstNode = lastNode = NULL;
    listSize = 0;
}

template <class T>
chain<T>::chain(const chain<T> &theList)
{
    listSize = theList.size();
    if (listSize == 0)
    { // 如链表为空
        firstNode = lastNode = NULL;
        return;
    }
    // 如链表非空
    chainNode<T> *sourceNode = theList.firstNode;      // 先定位被复制链表结点
    firstNode = new chainNode<T>(sourceNode->element); // 复制结点到本链表中
    sourceNode = sourceNode->next;
    chainNode<T> *targetNode = firstNode; // 该指针指向生成链表*this的最末端结点
    while (sourceNode != NULL)
    {                                                             // 遍历复制剩余所有元素
        targetNode->next = new chainNode<T>(sourceNode->element); // 复制节点
        targetNode = targetNode->next;
        sourceNode = sourceNode->next; // 移动两个指针
    }
    targetNode->next = NULL; // 尾结点指针域为NULL
    lastNode = targetNode;
}

template <class T>
chain<T>::~chain()
{ // 删除动态创建的结点
    while (firstNode != NULL)
    { // 删除头部，移动指针
        chainNode<T> *nextNode = firstNode->next;
        delete firstNode;
        firstNode = nextNode;
    }
}

template <class T>
T &chain<T>::get(int theIndex) const
{
    checkIndex();
    chainNode<T> *currentNode = firstNode;
    for (int i = 0; i < theIndex; i++)
        currentNode = currentNode->next; // 遍历链表，直到到达索引位置
    return currentNode->element;
}

template <class T>
int chain<T>::indexOf(const T &theElement) const
{
    chainNode<T> *currentNode = firstNode;
    int index = 0;
    for (; currentNode != NULL && currentNode->element != theElement; index++) // 除非到结尾或查找到，循环
        currentNode = currentNode->next;
    if (currentNode == NULL) // 没找到
        return -1;
    else
        return index;
}

template <class T>
void chain<T>::erase(int theIndex)
{
    checkIndex(theIndex);

    chainNode<T> *deleteNode;
    if (theIndex == 0) // 说明要删头节点
    {
        deleteNode = firstNode;
        firstNode = firstNode->next;
        if (firstNode == NULL) // 只有一个元素还被删了
            lastNode = NULL;
    }
    else // 说明要删一般节点
    {
        chainNode<T> *frontNode = firstNode; // 存储前置结点地址
        for (int i = 0; i < theIndex - 1; i++)
            frontNode = frontNode->next;    // 移动前置指针到前一个位置
        deleteNode = frontNode->next;       // 指向待删除结点
        frontNode->next = deleteNode->next; // 链接前后节点
        if (frontNode->next == NULL)        // 如果被删的是尾结点
            lastNode = frontNode;           // 确定新的尾结点
    }
    listSize--;
    delete deleteNode;
}

template <class T>
void chain<T>::insert(int theIndex, const T &theElement)
{
    if (theIndex == 0) // 插入新的头节点
        firstNode = new chainNode<T>(theElement, firstNode);
    else
    {
        chainNode<T> *frontNode = firstNode; // 存储前置结点地址
        for (int i = 0; i < theIndex - 1; i++)
            frontNode = frontNode->next;                                 // 移动前置指针到前一个位置
        frontNode->next = new chainNode<T>(theElement, frontNode->next); // 插入新节点
        if (frontNode->next->next == NULL)                               // 如果新插入的是尾结点
            lastNode = frontNode->next;
    }
    listSize++;
}

template <class T>
void chain<T>::reverse()
{
    if (firstNode == NULL || firstNode->next == NULL)
        return; // 没有或只有一个元素
    else
    {
        chainNode<T> *p = NULL;
        chainNode<T> *m = firstNode;
        chainNode<T> *q = firstNode->next;
        while (m != NULL)
        {
            m->next = p;
            p = m; // 移动各个指针
            m = q;
            if (q != NULL)
                q = q->next;
        }
        lastNode = firstNode; // 尾结点地址更改为头节点，头节点反之
        firstNode = p;
    }
}

template <class T>
void chain<T>::push_back(const T &theElement)
{
    chainNode<T> *newNode = new chainNode<T>(theElement, NULL);
    if (firstNode == NULL) // 如链表为空，直接加入
        firstNode = lastNode = newNode;
    else
    {
        lastNode->next = newNode;
        lastNode = newNode;
    }
    listSize++;
}

template <class T>
void chain<T>::output(ostream &out) const
{
    for (chainNode<T> *currentNode = firstNode; currentNode != NULL; currentNode = currentNode->next)
        out << currentNode->element << " ";
}

template <class T>
ostream &operator<<(ostream &out, const chain<T> &x)
{ // 重载输出运算符
    x.output(out);
    return out;
}

template <class T>
void chain<T>::set(int theIndex, T &theElement)
{
    checkIndex(theIndex);
    chain<T>::iterator it = this->begin();
    for (int i = 0; i < theIndex; i++, it++)
        ;
    *it = theElement;
}

#endif