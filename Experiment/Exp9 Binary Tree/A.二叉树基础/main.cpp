#include <iostream>

using namespace std;

// 为了实现遍历的非递归，我们需要引入栈和队列的定义
template <class T>
class Stack
{
public:
    virtual ~Stack() {}
    // 抽象类的析构函数
    virtual bool empty() const = 0;
    // 当栈为空时返回true
    virtual int size() const = 0;
    // 返回栈中元素个数
    virtual T &top() = 0;
    // 返回栈顶元素的引用
    virtual void pop() = 0;
    // 删除栈顶元素
    virtual void push(const T &theElement) = 0;
    // 将元素压入栈
};

template <class T>
class arrayStack : public Stack<T>
{
private:
    int stackTop;
    int arrayLength;
    T *stack;

public:
    arrayStack(int initialCapacity = 10);
    ~arrayStack() { delete[] stack; }
    bool empty() const { return stackTop == -1; }
    int size() const { return stackTop + 1; }
    T &top()
    {
        if (stackTop == -1)
            exit(0);
        return stack[stackTop];
    }
    void pop()
    {
        if (stackTop == -1)
            exit(0);
        stack[stackTop--].~T(); // 调用析构函数
    }
    void push(const T &theElement);
};

template <class T>
arrayStack<T>::arrayStack(int initialCapacity)
{
    if (initialCapacity < 1)
        exit(0);
    arrayLength = initialCapacity;
    stack = new T[arrayLength];
    stackTop = -1;
}

template <class T>
void arrayStack<T>::push(const T &theElement)
{
    if (stackTop == arrayLength - 1) // 栈满了，加倍空间
    {
        arrayLength *= 2;
        T *newStack = new T[arrayLength];
        for (int i = 0; i <= stackTop; i++)
            newStack[i] = stack[i];
        delete[] stack;
        stack = newStack;
    }
    // 栈顶压入元素
    stack[++stackTop] = theElement;
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
            // no wrap around
            std::copy(queue + start, queue + start + arrayLength - 1, newQueue);
        else
        { // queue wraps around
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

template <class T>
struct binaryTreeNode
{
    T element;
    binaryTreeNode<T> *leftChild;  // 左子树指针
    binaryTreeNode<T> *rightChild; // 右子树指针

    binaryTreeNode() { leftChild = rightChild = NULL; }
    binaryTreeNode(const T &theElement)
    {
        element = theElement;
        leftChild = rightChild = NULL;
    }
    binaryTreeNode(const T &theElement, binaryTreeNode<T> *theLeftChild, binaryTreeNode<T> *theRightNode)
    {
        element = theElement;
        leftChild = theLeftChild;
        rightChild = theRightNode;
    }
};

template <class T>
class linkedBinaryTree
{
private:
    binaryTreeNode<T> root; // 根节点指针
    int treeSize;           // 树结点的数目

public:
    linkedBinaryTree() // 构造函数
    {
        root = NULL;
        treeSize = 0;
    }
    bool empty() const { return treeSize == 0; } // 判空函数
    int size() const { return size; }
    void preOrder(binaryTreeNode<T> *t);   // 前序遍历
    void inOrder(binaryTreeNode<T> *t);    // 中序遍历
    void postOrder(binaryTreeNode<T> *t);  // 后序遍历
    void levelOrder(binaryTreeNode<T> *t); // 层次遍历
    int height(binaryTreeNode<T> *t);      // 返回树的高度
    int num(binaryTreeNode<T> *t);         // 返回树的节点数
};

template <class T>
void linkedBinaryTree<T>::preOrder(binaryTreeNode<T> *t)
{
    if (t == NULL)
        return;
    cout << t->element << ' '; // 输出根节点
    preOrder(t->leftChild);    // 遍历搜左孩子
    preOrder(t->rightChild);   // 遍历搜右孩子
}

template <class T>
void linkedBinaryTree<T>::inOrder(binaryTreeNode<T> *t)
{
    arrayStack<binaryTreeNode<T> *> stack; // 构建结点栈
    binaryTreeNode<T> *p = t;              // 遍历指针
    while (p != NULL || !stack.empty())    // 在指针或栈非空时持续循环
    {
        if (p != NULL) // 指针非空
        {
            stack.push(p);    // 先存根节点
            p = p->leftChild; // 在存左孩子
        }
        else // 指针为空了，没有后面的左子树了
        {
            p = stack.top(); // 往前退一结点
            stack.pop();
            cout << p->element << " "; // 输出当前结点
            p = p->rightChild;         // 开搜右子树
        }
    }
}

template <class T>
void linkedBinaryTree<T>::postOrder(binaryTreeNode<T> *t)
{
    arrayStack<binaryTreeNode<T> *> stack;
    binaryTreeNode<T> *p = t;
    binaryTreeNode<T> *pp; // 辅助指针
    while (p != NULL || !stack.empty())
    {
        if (p != NULL)
        {
            stack.push(p);
            p = p->leftChild;
        }
        else
        {
            p = stack.top();
            if (p->rightChild != NULL && p->rightChild != pp) // 右子树存在且没有被访问过时
            {
                p = p->rightChild;
                stack.push(p);
                p = p->leftChild;
            }
            else
            {
                stack.pop();
                cout << p->element << " ";
                pp = p;
                p = NULL;
            }
        }
    }
}

template <class T>
void linkedBinaryTree<T>::levelOrder(binaryTreeNode<T> *t)
{
    arrayQueue<binaryTreeNode<T> *> queue; // 创建结点队列
    binaryTreeNode<T> *p = t;              // 遍历指针

    while (p != NULL)
    {
        cout << p->element << " "; // 直接输出该结点元素

        if (p->leftChild != NULL) // 当有左孩子时
            queue.push(p->leftChild);
        if (p->rightChild != NULL) // 当有右孩子时
            queue.push(p->rightChild);
        if (queue.empty())
            break;
        p = queue.front();
        queue.pop();
    }
}

template <class T>
int linkedBinaryTree<T>::height(binaryTreeNode<T> *t)
{
    if (t == NULL)
        return 0;
    return max(height(t->rightChild), height(t->leftChild)) + 1; // 递归求解树
}

template <class T>
int linkedBinaryTree<T>::num(binaryTreeNode<T> *t)
{
    int n = 0;
    if (t != NULL) // 递归求解
        n = num(t->leftChild) + num(t->rightChild) + 1;
    return n;
}

int main()
{
    int n;
    cin >> n;
    binaryTreeNode<int> **NodeList = new binaryTreeNode<int> *[n];
    for (int i = 1; i <= n; i++)
        NodeList[i] = new binaryTreeNode<int>(i);
    for (int i = 1; i <= n; i++) // 初始化结点序列
    {
        int l, r; // 左右节点序号
        cin >> l >> r;
        if (l != -1)
            NodeList[i]->leftChild = NodeList[l];
        else
            NodeList[i]->leftChild = NULL;
        if (r != -1)
            NodeList[i]->rightChild = NodeList[r];
        else
            NodeList[i]->rightChild = NULL;
    }
    linkedBinaryTree<int> tree;
    tree.preOrder(NodeList[1]);
    cout << endl;
    tree.inOrder(NodeList[1]);
    cout << endl;
    tree.postOrder(NodeList[1]);
    cout << endl;
    tree.levelOrder(NodeList[1]);
    cout << endl;
    for (int i = 1; i <= n; i++)
        cout << tree.num(NodeList[i]) << " ";
    cout << endl;
    for (int i = 1; i <= n; i++)
        cout << tree.height(NodeList[i]) << " ";
    cout << endl;

    delete[] NodeList;

    return 0;
}
