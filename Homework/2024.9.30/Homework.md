# 数据结构与算法作业  2024/9/30

首先，在完成作业前，我们创建如下链表类定义，具体方法实现略。

```c++
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
    void push_back(const T &theElement);            // 链表末尾插入
    void output(ostream &out) const;                // 向输出流输出内容

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
```

## 1.（P124.T3）
解：在类`chain<T>`中，我们创建方法`set(int theindex,T &theREement)`，方法实现如下：
```c++
template <class T>
void chain<T>::set(int theIndex, T &theElement)
{
    checkIndex(theIndex);
    chain<T>::iterator it = this->begin();
    for (int i = 0; i < theIndex; i++, it++)
        ;
    *it = theElement;
}
```
利用`checkIndex()`函数，我们可以检测我们传入的索引是否合法，并在不合法的情况下抛出异常。而利用我们创建的迭代器。我们可以检索到索引对应的结点，并利用解引用运算符来实现结点内容的修改，从而实现了`set()`方法。在迭代器实现的基础上，该方法时间复杂度为$O(theIndex)$。

利用下主函数，我们可以测试该方法的正确性：
```c++
int main()
{
    chain<int> ch;
    for (int i = 0; i < 5; i++)
        ch.push_back(i);
    int a = 999;
    ch.set(1, a);
    ch.output(cout);
    return 0;
}
```
最终输出结果为：0 999 2 3 4。这证明了我们方法的正确性。


## 2.（P125.T15）

解：在类`chain<T>`中，我们创建方法`reverse()`，方法实现如下：
```c++
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
```
该算法的本质是通过改变链表各个结点的指针域来实现链表指向的整体反转，并通过设置头指针为原尾结点地址，来将原链表进行了反转，并没有分配新的节点空间。

由于该算法的实现过程，我们只是遍历了链表，因此时间复杂度为$O(listSize)$

为验证该方法的正确性，我们编写如下主函数进行测试：
```c++
int main()
{
    chain<int> ch;
    for (int i = 0; i < 5; i++)
        ch.push_back(i);
    ch.reverse();
    ch.output(cout);
    return 0;
}
```
最终输出结果为：4 3 2 1 0。这证明了我们方法的正确性。

## 3.（P125.T17）

解：我们创建方法`void meld(chain<T> &a, chain<T> &b, chain<T> &c)`，它可以将a与b两个链表的元素交错放置在链表c中，形成组合新链表，方法实现如下：
```c++
template <class T>
void meld(chain<T> &a, chain<T> &b, chain<T> &c)
{ // 把a，b两个链表的数据轮流插入c中
    // 创建三个链表的迭代器
    chain<int>::iterator ap = a.begin();
    chain<int>::iterator bp = b.begin();
    chain<int>::iterator cp = c.begin();
    while (ap != NULL || bp != NULL)
    {
        if (ap != NULL) // 先尝试从a插
        {
            c.push_back(*ap);
            ap++;
        }
        if (bp != NULL) // 再尝试从b插
        {
            c.push_back(*bp);
            bp++;
        }
    }
}
```
其具体实现便是利用两个链表的迭代器，通过轮流取值插入并移动的方式实现交错合并，而当某迭代器对应空指针时，说明该链表已插入完毕，可以不再插入。

由于该算法我们只执行了单层循环，且我们进行的只是将两个链表的每个元素插入新链表，该方法最终执行总步数为$a.listSize+b.listSize$，因此，该算法的时间复杂度为$O(n)$，也就是说，其具有线性复杂度。

为验证该方法的正确性，我们编写如下主函数进行测试：
```c++
int main()
{
    chain<int> a, b, c;
    for (int i = 0; i < 5; i++)
    {
        a.push_back(i);
        b.push_back(i + 5);
    }
    a.erase(4);
    a.erase(3);
    meld(a, b, c);
    c.output(cout);
}
```
最终输出结果为：0 5 1 6 2 7 8 9。这证明了我们方法的正确性。