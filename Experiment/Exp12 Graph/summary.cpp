#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

template <class T>
class arrayList
{
private:
    T *element;      // 储存元素的一维数组
    int arrayLength; // 数组容量
    int listSize;    // 元素个数
public:
    arrayList(int initialCapacity = 10); // 普通构造函数
    arrayList(const arrayList<T> &arr);  // 复制构造函数
    ~arrayList() { delete[] element; }

    bool empty() const { return listSize == 0; }
    int size() { return listSize; }

    void checkIndex(int theIndex) const;            // 检查索引
    T &get(int theIndex);                           // 获取元素
    int indexOf(const T &theElement) const;         // 获取索引
    void insert(int theIndex, const T &theElement); // 插入函数
    void erase(int theIndex);                       // 删除函数
    void push_back(const T &theElement);            // 尾插入函数
    void output(ostream &out) const;                // 创建输出流
    void arr_sort();                                // 排序本数组
    void clear();                                   // 清空数组
    void reset(int length, T value);                // 重设数组

    // 运算符重载
    // 重载 [] 运算符
    T &operator[](int index);
    const T &operator[](int index) const;
    // 重载 * 运算符
    T &operator*() { return element[0]; };
    const T &operator*() const { return element[0]; };
    // 重载类型转换运算符，将对象转换为指针
    operator T *() { return element; };
    operator const T *() const { return element; };
};

template <class T>
arrayList<T>::arrayList(int initialCapacity)
{ // 构造函数
    arrayLength = initialCapacity;
    element = new T[arrayLength];
    listSize = 0;

    // 初始化所有元素为默认值
    for (int i = 0; i < arrayLength; ++i)   
    {
        element[i] = T(); // 使用默认构造函数初始化每个元素
    }
}

template <typename T>
arrayList<T>::arrayList(const arrayList<T> &theList)
{ // 复制构造函数
    arrayLength = theList.arrayLength;
    listSize = theList.listSize;
    element = new T[arrayLength];
    std::copy(theList.element, theList.element + listSize, element);
}

template <typename T>
void arrayList<T>::checkIndex(int theIndex) const
{
    if (theIndex < 0 || theIndex >= listSize)
    {
        std::ostringstream s;
        s << "index = " << theIndex << "size = " << listSize;
        std::cout << s.str() << std::endl;
        exit(0);
    }
}

template <typename T>
T &arrayList<T>::get(int theIndex)
{ // 若存在返回theIndex的元素
    checkIndex(theIndex);
    return element[theIndex];
}

template <class T>
inline int arrayList<T>::indexOf(const T &theElement) const
{
    for (int i = 0; i < listSize; i++)
        if (element[i] == theElement)
            return i;
    return -1;
}

template <class T>
inline void arrayList<T>::insert(int theIndex, const T &theElement)
{
    if (listSize == arrayLength)
    {                     // 如果线性表满了
        arrayLength *= 2; // 二倍扩大线性表大小
        T *newElement = new T[arrayLength];
        for (int i = 0; i < listSize; ++i)
        {
            newElement[i] = element[i];
        }
        delete[] element; // 释放旧数组
        element = newElement;
    }
    if (theIndex == listSize - 1) // 在尾部直接尾插入
        element[listSize++] = theElement;
    else // 否则需要移动
    {
        copy_backward(element + theIndex, element + listSize,
                      element + listSize + 1);
        element[theIndex] = theElement;
        listSize++;
    }
}

template <typename T>
void arrayList<T>::erase(int theIndex)
{
    checkIndex(theIndex);
    std::copy(element + theIndex + 1, element + listSize, element + theIndex);
    element[--listSize].~T();
}

template <typename T>
void arrayList<T>::push_back(const T &theElement)
{
    if (listSize == arrayLength)
    {                     // 如果线性表满了
        arrayLength *= 2; // 二倍扩大线性表大小
        T *newElement = new T[arrayLength];
        for (int i = 0; i < listSize; ++i)
        {
            newElement[i] = element[i];
        }
        delete[] element; // 释放旧数组
        element = newElement;
    }
    element[listSize++] = theElement; // 尾插入
}

template <class T>
inline void arrayList<T>::output(ostream &out) const
{
    copy(element, element + listSize, ostream_iterator<T>(cout, " "));
}

template <class T>
inline void arrayList<T>::arr_sort()
{
    sort(element, element + listSize);
}

template <class T>
void arrayList<T>::clear()
{
    listSize = 0; // 将数组大小重置为0，逻辑上清空列表
}

template <class T>
inline void arrayList<T>::reset(int length, T value)
{
    this->clear();
    for (int i = 0; i < length; i++)
        this->push_back(value);
}

template <class T>
inline T &arrayList<T>::operator[](int index)
{
    checkIndex(index); // 检查索引是否合法
    return element[index];
}

template <class T>
inline const T &arrayList<T>::operator[](int index) const
{
    checkIndex(index); // 检查索引是否合法
    return element[index];
}

template <class T>
ostream &operator<<(ostream &out, const arrayList<T> &x)
{ // 重载输出运算符
    x.output(out);
    return out;
}

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
    chain<T> &operator=(const chain<T> &theList);
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

    // 迭代器嵌套类
    class iterator;
    iterator begin() { return iterator(firstNode); } // 返回头部迭代器
    iterator end() { return iterator(NULL); }        // 返回尾部迭代器（为空）

    // 链表类迭代器的定义
    class iterator
    {
    public:
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
chain<T> &chain<T>::operator=(const chain<T> &theList)
{
    if (this != &theList)
    {
        // 删除旧链表
        chainNode<T> *current = firstNode;
        while (current != nullptr)
        {
            chainNode<T> *nextNode = current->next;
            delete current;
            current = nextNode;
        }

        // 拷贝新链表
        listSize = theList.listSize;
        if (theList.firstNode == nullptr)
            firstNode = lastNode = nullptr;
        else
        {
            firstNode = new chainNode<T>(theList.firstNode->element);
            chainNode<T> *current = firstNode;
            chainNode<T> *sourceNode = theList.firstNode->next;
            while (sourceNode != nullptr)
            {
                current->next = new chainNode<T>(sourceNode->element);
                current = current->next;
                sourceNode = sourceNode->next;
            }
            lastNode = current;
        }
    }
    return *this;
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
    checkIndex(theIndex);
    chainNode<T> *currentNode = firstNode;
    for (int i = 0; i < theIndex; i++)
        currentNode = currentNode->next; // 遍历链表，直到到达索引位置
    return currentNode->element;
}

template <class T>
int chain<T>::indexOf(const T &theElement) const
{
    // 如果链表为空，直接返回 -1
    if (firstNode == NULL)
        return -1;

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
    if (theIndex < 0 || theIndex > listSize) // 修复：检查插入索引范围
        throw illegalIndex("Invalid index for insert");

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
class queue
{
public:
    virtual ~queue() {};
    // 析构函数
    virtual bool empty() const = 0;
    // 返回true，当且仅当队列为空
    virtual int size() const = 0;
    // 返回队列中元素个数
    virtual T &front() = 0;
    // 返回头元素的引用
    virtual T &back() = 0;
    // 返回尾元素的引用
    virtual void pop() = 0;
    // 删除头元素
    virtual void push(const T &theElement) = 0;
    // 吧元素加入队列尾
};

template <class T>
class arrayQueue : public queue<T>
{
public:
    arrayQueue(int initialCapacity = 10);
    ~arrayQueue() { delete[] queue; }
    bool empty() const { return theFront == theBack; }
    int size() const
    {
        return (theBack - theFront + arrayLength) % arrayLength;
    }
    T &front()
    { // 返回头元素引用
        if (theFront == theBack)
            exit(0);
        return queue[(theFront + 1) % arrayLength];
    }
    T &back()
    { // 返回尾元素引用
        if (theFront == theBack)
            exit(0);
        return queue[theBack];
    }
    void pop()
    { // 删除首元素
        if (theFront == theBack)
            exit(0);
        theFront = (theFront + 1) % arrayLength; // 向前移动头索引
        queue[theFront].~T();                    // 删除原本头元素
    }
    void push(const T &theElement);
    void output() // 输出队列
    {
        int index = (theFront + 1) % arrayLength;
        while (index % arrayLength != (theBack + 1) % arrayLength)
        {
            std::cout << queue[index] << std::endl;
            index++;
        }
    }

private:
    int theFront;    // 循环队列顺时针方向上的首元素索引
    int theBack;     // 尾元素索引
    int arrayLength; // 队列长度
    T *queue;        // 队列元素数组指针
};

template <class T>
arrayQueue<T>::arrayQueue(int initialCapacity)
{ // 构造函数实现
    if (initialCapacity < 1)
        exit(0);
    arrayLength = initialCapacity;
    queue = new T[arrayLength];
    theFront = 0;
    theBack = 0;
}

template <class T>
void arrayQueue<T>::push(const T &theElement)
{ // 队列尾部压入新元素

    // 在必要条件下扩增数组空间
    if ((theBack + 1) % arrayLength == theFront)
    { // 加倍数组空间
        // 创建新数组
        T *newQueue = new T[2 * arrayLength];

        // 将原本元素加入其中
        int start = (theFront + 1) % arrayLength;
        if (start < 2)
            // 假定原队列没绕回来
            std::copy(queue + start, queue + start + arrayLength - 1, newQueue);
        else
        { // 假如绕回来了
            std::copy(queue + start, queue + arrayLength, newQueue);
            std::copy(queue, queue + theBack + 1, newQueue + arrayLength - start);
        }

        // 切换为新数组，重设队列参数
        theFront = 2 * arrayLength - 1;
        theBack = arrayLength - 2; // 队列大小为arrayLength - 1，因此theBack为队列大小 - 1
        arrayLength *= 2;
        queue = newQueue;
    }

    // 将元素放至队列末尾
    theBack = (theBack + 1) % arrayLength;
    queue[theBack] = theElement;
}

// 以上都是模板类，下面才是图论实验的重头戏

template <class T>
class graphChain : public chain<T>
{
public:
    T *eraseElement(int theVertex)
    {
        int index = this->indexOf(theVertex);
        this->erase(index);
    }

    void sortList()
    {
        arrayList<T> temp;
        // 遍历链表，将元素加入到 tempList 中
        for (chainNode<T> *node = this->firstNode; node != NULL; node = node->next)
            temp.push_back(node->element);
        temp.arr_sort();
        this->firstNode = NULL;
        for (int i = 0; i < temp.size(); i++)
            this->push_back(temp[i]);
    }
};

class linkedGraph
{
protected:
    graphChain<int> *aList; // 邻接表
    int n;                  // 节点数
    int e;                  // 边数
public:
    linkedGraph(int num_of_Vertices = 0);
    ~linkedGraph() { delete[] aList; }
    void output()
    {
        for (int i = 1; i <= n; i++)
            cout << "节点" << i << ":" << aList[i] << endl;
        cout << endl;
    }

    // 对边的处理
    bool existEdge(int i, int j);  // 判断边是否存在
    void insertEdge(int i, int j); // 插入边
    void eraseEdge(int i, int j);  // 删除边
    void sortAllLists()
    {
        for (int i = 1; i <= n; i++)
            aList[i].sortList();
    }

    // 搜索函数：bfs和dfs
    int bfs(int start, arrayList<int> &result, arrayList<int> &reach);  // 广度优先搜索，得到字典序最小序列和到达点标记，返回序列长度
    void dfs(int start, arrayList<int> &result, arrayList<int> &reach); // 深度优先搜索，得到字典序最小序列和到达点标记

    // 连通分支相关
    int countConnectedComponents(arrayList<int> &minNodes); // 计算图中连通分量的个数，同时得到最小节点序列

    // 最短路径相关
    int getShortestPath(int start, int end);
};

int main()
{
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    // 图的创建
    linkedGraph graph(n);
    for (int i = 0; i < m; i++)
    {
        int op, u, v;
        cin >> op >> u >> v;
        if (op == 0)
            graph.insertEdge(u, v);
        else if (op == 1)
            graph.eraseEdge(u, v);
        // graph.output();
    }

    graph.sortAllLists();

    // 输出连通分量个数和最小点编号
    arrayList<int> minNodes;
    cout << graph.countConnectedComponents(minNodes) << endl;
    cout << minNodes << endl;
    // 输出dfs长度和序列
    arrayList<int> result;
    arrayList<int> reach;
    reach.reset(n + 1, 0);
    graph.dfs(s, result, reach);
    cout << result.size() << endl;
    cout << result << endl;
    // 输出bfs长度和序列
    result.clear();
    reach.reset(n + 1, 0);
    cout << graph.bfs(t, result, reach) << endl;
    cout << result << endl;
    // 寻找最短路径
    cout << graph.getShortestPath(s, t) << endl;
    return 0;
}

linkedGraph::linkedGraph(int num_of_Vertices)
{

    if (num_of_Vertices < 0)
        exit(0);
    n = num_of_Vertices;
    e = 0;
    aList = new graphChain<int>[n + 1]; // 第一个元素没用上
}

bool linkedGraph::existEdge(int i, int j)
{
    if (i > n || j > n || i < 1 || j < 1)
        return false;
    graphChain<int> *V = &aList[i];
    int flag = V->indexOf(j);
    if (flag == -1)
        return false;
    return true;
}

void linkedGraph::insertEdge(int i, int j)
{
    bool flag = existEdge(i, j);
    if (!flag)
    {
        aList[i].push_back(j);
        aList[j].push_back(i);
        e++;
    }
}

void linkedGraph::eraseEdge(int i, int j)
{
    bool flag = existEdge(i, j);
    if (flag)
    {
        aList[i].eraseElement(j);
        aList[j].eraseElement(i);
        e--;
    }
}

int linkedGraph::bfs(int start, arrayList<int> &result, arrayList<int> &reach)
{
    arrayQueue<int> q(10);
    reach[start] = 1;
    q.push(start);

    while (!q.empty())
    {
        // 从队列中删除一个有标记的顶点
        int w = q.front();
        q.pop();

        // 访问节点加入结果数组
        result.push_back(w);

        // 遍历 w 的邻接节点
        for (chain<int>::iterator i = aList[w].begin(); i != aList[w].end(); i++)
        {
            if (reach[*i] == 0) // 未访问的邻接节点
            {
                q.push(*i);
                reach[*i] = 1; // 标记为到达
            }
        }
    }

    return result.size();
}

void linkedGraph::dfs(int start, arrayList<int> &result, arrayList<int> &reach)
{
    // 访问当前节点，加入结果数组并标记为已访问
    reach[start] = 1;
    result.push_back(start);

    // 遍历邻接节点
    for (chain<int>::iterator i = aList[start].begin(); i != aList[start].end(); i++)
    {
        int next = *i;
        if (reach[next] == 0) // 如果未访问
        {
            dfs(next, result, reach); // 递归深度优先遍历
        }
    }
}

// 计算连通分量的数量
int linkedGraph::countConnectedComponents(arrayList<int> &minNodes)
{
    arrayList<int> reach(n + 1);
    arrayList<int> result;
    reach.reset(n + 1, 0);
    int componentCount = 0; // 连通分量计数

    // 遍历每个节点，进行DFS或BFS
    for (int i = 1; i <= n; i++)
    {
        if (reach[i] == 0)
        {
            // 以每个未访问的节点为起点，开始DFS
            result.clear();
            dfs(i, result, reach);
            minNodes.push_back(i);
            componentCount++;
        }
    }
    return componentCount;
}

// 最短路径可以用BFS来找
int linkedGraph::getShortestPath(int start, int end)
{
    if (start == end)
        return 0;

    arrayQueue<int> q(10);          // 队列用于 BFS
    arrayList<int> reach(n + 1);    // 记录节点是否已访问
    arrayList<int> distince(n + 1); // 记录每个节点的路径
    reach.reset(n + 1, 0);
    distince.reset(n + 1, 0);

    q.push(start);
    reach[start] = 1; // 标记起始节点已访问
    distince[start] = 0;

    while (!q.empty())
    {
        int current = q.front(); // 当前节点
        q.pop();

        // 遍历当前节点的邻接节点
        arrayList<int> neighbors;
        for (chain<int>::iterator i = aList[current].begin(); i != aList[current].end(); i++)
            neighbors.push_back(*i);

        // 遍历邻接节点
        for (int i = 0; i < neighbors.size(); i++)
        {
            int neighbor = neighbors[i];
            if (reach[neighbor] == 0)
            { // 如果节点没有被访问过
                q.push(neighbor);
                reach[neighbor] = 1;                        // 标记该节点已访问
                distince[neighbor] = distince[current] + 1; // 子节点距离为父节点+1

                // 如果找到了目标节点，返回
                if (neighbor == end)
                    return distince[neighbor];
            }
        }
    }

    return -1; // 如果没有找到路径
}
