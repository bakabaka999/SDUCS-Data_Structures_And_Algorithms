# 数据结构与算法作业  2024/10/14

## 1.(P163.T32)
解：我们为了实现将下三角矩阵与上三角矩阵做乘积，我们需要创建三个：
上三角矩阵类、下三角矩阵类以及一般矩阵类，三种类的定义如下：

下三角矩阵类：
```c++
// 下三角矩阵类
class LowerTriangularMatrix
{
private:
    double *data; // 一维数组存储
    int n;        // 矩阵维度
public:
    LowerTriangularMatrix(int size) // 构造函数
    {
        n = size;
        data = new double[n * (n + 1) / 2]; // 动态开辟数组空间
    }
    ~LowerTriangularMatrix() // 析构函数
    {
        delete[] data;
    }

    // 设置元素值
    void set(int i, int j, double value)
    {
        if (i >= j) // 因为是下三角矩阵
            data[i * (i + 1) / 2 + j] = value;
    }
    // 获取元素值
    double get(int i, int j)
    {
        return (i >= j) ? data[i * (i + 1) / 2 + j] : 0;
    }
    int size()
    {
        return n;
    }
};
```

上三角矩阵类：
```c++
// 上三角矩阵类
class UpperTriangularMatrix
{
private:
    double *data; // 一维数组存储
    int n;        // 矩阵维度
public:
    UpperTriangularMatrix(int size) // 构造函数
    {
        n = size;
        data = new double[n * (n + 1) / 2]; // 动态开辟数组空间
    }
    ~UpperTriangularMatrix() // 析构函数
    {
        delete[] data;
    }

    // 设置元素值
    void set(int i, int j, double value)
    {
        if (i <= j) // 因为是上三角矩阵
            data[i * n - i * (i - 1) / 2 + (j - i)] = value;
    }
    // 获取元素值
    double get(int i, int j)
    {
        return (i <= j) ? data[i * n - i * (i - 1) / 2 + (j - i)] : 0;
    }
    int size()
    {
        return n;
    }
};
```

普通矩阵类：
```c++
// 普通矩阵类
class Matrix
{
private:
    double **data; // 二维指针存储二维数组
    int n;         // 矩阵维度
public:
    // 构造函数
    Matrix(int size)
    {
        n = size;
        data = new double *[n];
        for (int i = 0; i < n; i++)
        {
            data[i] = new double[n];
            for (int j = 0; j < n; j++)
                data[i][j] = 0; // 初始化为0
        }
    }
    // 析构函数
    ~Matrix()
    {
        for (int i = 0; i < n; i++)
            delete[] data[i];
        delete[] data;
    }

    // 设置元素值
    void set(int i, int j, int value)
    {
        data[i][j] = value;
    }
    // 获取元素值
    double get(int i, int j)
    {
        return data[i][j];
    }
    // 获取大小
    int size()
    {
        return n;
    }
    // 输出矩阵
    void output()
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                cout << data[i][j] << " ";
            cout << endl;
        }
    }
};
```

在这基础上，我们构建函数`void multiply(LowerTriangularMatrix &low, UpperTriangularMatrix up, Matrix &m)`，该函数将储存在low和high中的三角矩阵作乘积，并将结果存储到矩阵m中：
```c++
// 乘法运算函数
void multiply(LowerTriangularMatrix &low, UpperTriangularMatrix up, Matrix &m)
{
    int n = low.size();
    for (int i = 0; i < n; i++) // 遍历下三角矩阵的行
    {
        for (int j = 0; j < n; j++) // 遍历上三角矩阵的列
        {
            int sum_result = 0;
            for (int k = 0; k <= i; k++)
                sum_result += low.get(i, k) * up.get(k, j);
            m.set(i, j, sum_result);
        }
    }
}
```

该算法的时间复杂度显然为$O(n^3)$，更严格来说的话，是$\frac{n^2(n+1)}{2}$，也就是说，是优于一般矩阵乘法的直接相乘方法。从空间复杂度上来看更是如此。

经过主函数测试，我们证明该函数编写正确。

## 2.(P184.T12)

解：

首先，该题我们用到的linkedStack类定义如下：

```c++
template <class T>
class linkedStack : public Stack<T>
{
private:
    chainNode<T> *stackTop; // 栈顶指针
    int stackSize;          // 栈内元素个数
public:
    linkedStack(int initialCapacity = 10);
    ~linkedStack();
    bool empty() const { return stackSize == 0; }
    int size() const { return stackSize; }
    T &top();
    void pop();
    void push(const T &theElement);

    // 作业函数
    void pushNode(chainNode<T>* theNode)
    {
        theNode->next = stackTop;
        stackTop = theNode;
    }
    chainNode<T>* popNode()
    {
        chainNode<T> *returnNode = stackTop;
        stackTop = stackTop->next;
        return returnNode;
    }

};
```

1) 该题要求给出的两个函数如下所示：
```c++
void pushNode(chainNode<T>* theNode)
    {
        theNode->next = stackTop;
        stackTop = theNode;
    }
    chainNode<T>* popNode()
    {
        chainNode<T> *returnNode = stackTop;
        stackTop = stackTop->next;
        return returnNode;
    }
```
这两个函数我们定义为linkedStack类的成员函数。

2) 为测试我们编写的函数，我们编写主函数如下：
```c++
int main()
{
    linkedStack<int> int_stack;
    int_stack.push(1);
    int_stack.push(2);

    chainNode<int>* e = new chainNode<int>(114514);
    int_stack.pushNode(e);
    cout << int_stack.popNode()->element << endl;
    cout << int_stack.top() << endl;

    return 0;
}
```
经过测试，我们的pushNode确实可以压入指针，而且popNode也可以弹出栈顶结点指针。

程序输出为：114514  2

3) 在push函数中，我们利用传入的数据构建新节点在压入，因此，其时间复杂度为构建新节点的时间复杂度。而pushNode我们是将已有结点压入，时间复杂度为$O(1)$

    在pop函数中，我们是直接将栈顶结点删除，因此其时间复杂度为删除节点的时间复杂度。而popNode函数我们只是弹出指针并返回，并没有执行析构函数删除，因此，其时间复杂度也是$O(1)$



