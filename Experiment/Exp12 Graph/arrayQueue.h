#ifndef _ARRAY_QUEUE_H_
#define _ARRAY_QUEUE_H_

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

#endif