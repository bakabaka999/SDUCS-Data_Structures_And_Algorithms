# 数据结构与算法作业  2024/10/21

## 1.(P211.T5.1.iii)

解：在做这个题之前，我们首先完成一下几个类

1. `queue<T>`类

    该类是队列类的抽象类，定义了一个队列应当包含的基本方法，比如：判空，判元素个数，返回头、尾元素的引用，头删除，尾插入等，类定义如下：

```c++
// queue抽象基类
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
```

2. `arrayQueue<T>`类

    该类是队列基类的数组实现模板类，通过循环数组的方法实现了队列的基本功能，同时还加入了输出队列的方法，类定义如下：

```c++
// arrayQueue类
template <class T>
class arrayQueue : public queue<T>
{
public:
    arrayQueue(int initialCapacity = 10);
    ~arrayQueue() { delete[] queue; }
    bool empty() const { return theFront == theBack; }
    int size() const;
    T &front();
    T &back();
    void pop();
    void push(const T &theElement);
    void output(); // 输出队列

private:
    int theFront;    // 循环队列顺时针方向上的首元素索引
    int theBack;     // 尾元素索引
    int arrayLength; // 队列长度
    T *queue;        // 队列元素数组指针
};
```

在这基础上，我们为该类新增了一方法`break_down()`：可以将本队列分解为两个新队列，一个包含奇数号元素，另一个包含偶数号元素，函数的定义与实现如下所示：

```c++
template<class T>
void arrayQueue<T>::break_down(arrayQueue<T> &a, arrayQueue<T> &b)
{
    int flag = 1; // 奇偶标记
    for (int i = (theFront + 1) % arrayLength; i <= theBack; i = (i + 1) % arrayLength)
    {
        if (flag % 2 == 1)
            a.push(queue[i]);
        else
            b.push(queue[i]);
        flag++;
    }
}
```

在该函数中，我们循环遍历原队列，将原队列按照其位置奇偶性来实现元素的分配，将元素分配至传入的队列中。这一分配，我们可以采用队列的尾插入方法来实现。

其中值得注意的有两点：

1. 我们判断奇偶采用的是自创建的变量flag，这一变量在每次循环后+1，而其对2取模得到结果可以作为奇偶的判定，具体为：“为1为奇，为2为偶”。

2. 我们在计算元素索引时，应当记住的是，实际theFront索引对应的并不是第一个元素所在位置，而是第一个元素所在位置的前一个位置，且由于这是循环队列，我们在移动索引时，不能忘记对arrayLength取模，以得到我们在队列数组中实际对应元素的索引。这也是为什么，我们的循环索引从`(theFront + 1) % arrayLength`开始，到`theBack`结束，且在过程自增时，我们时刻对arrayLength取模。

## 2.(P258.T17)

解：

1) 整体的插入步骤示意图如下所示，其中，红色标识的是此步骤插入的关键字，蓝色标识的是此步骤插入的因冲突所以使用线性探查法解决冲突插入的关键字。
![alt text](image.png)

2) 设散列表桶数为$b$，插入元素个数为$n$，计算得到的负载因子为$\alpha$。由题目条件：$b=13$，$n=10$，因此：

$$
\alpha=\frac{n}{b}=\frac{10}{13}\approx0.77
$$

3) 由插入示意图，我们可以得知，对于一次失败的搜索，假定我们搜索的元素为$k$，其哈希值为$i$，即：$i=f(k)=k\%13$，那么对于$i\in[0,12]$，对应的搜索桶数应当为：

    $$
    [3,2,1,2,1,2,1,9,8,7,6,5,4]
    $$

    这是由于一次错误的搜索必然落至空白处或回到原位置。

    设搜索桶数为$U$，那么其最大搜索桶数$U_{max}$与平均搜索桶数$\bar{U}$为：

    $$
    U_{max}=9 \quad , \quad \bar{U}=\frac{51}{13}\approx3.9
    $$

4) 由插入示意图，我们可以得知，对于一次成功的搜索，对关键字序列$[7,42,25,70,14,38,8,21,34,11]$，对应的搜索桶数应当为：

    $$
    [1,1,1,1,1,2,1,2,3,1]
    $$

    设搜索桶数为$S$，那么其最大搜索桶数$S_{max}$与平均搜索桶数$\bar{S}$为：

    $$
    S_{max}=3 \quad , \quad \bar{S}=\frac{14}{10}=1.4
    $$

5) 由计算公式，我们计算得到$U_n$与$S_n$如下所示：
    $$
    U_n\approx\frac{1}{2}(1+\frac{1}{(1-\alpha)^2})\approx 9.95
    $$

    $$
    S_n\approx\frac{1}{2}(1+\frac{1}{(1-\alpha)})\approx 2.67
    $$


    很明显，该计算公式所得的结果比起实际结果是偏大的，这也证明的这一计算公式并非是完全准确的，但是，当$n$非常大时，该计算公式所得的结果将会更为接近实际结果。