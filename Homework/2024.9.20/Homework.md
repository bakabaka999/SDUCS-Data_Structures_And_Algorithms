# 数据结构与算法作业  2024/9/20

## 1.(P105.T11)

解：最终编写程序建立在以下`arrayList`类上：
```c++
#include<bits/stdc++.h>

template <typename T>
// 抽象类
class linearList
{
    // 内容略
}

template <class T>
class arrayList : public linearList<T>
{
public:
    // 已有代码略

    // 作业新增函数
    void push_back(const T &theElement);
    void clear();

protected:
    void checkIndex(int theIndex) const;
    T *element;      // 存储线性表元素的一位数组
    int arrayLength; // 一维数组的容量
    int listSize;    // 线性表元素的个数
};

```
其中，`arrayList<T>::push_back()`函数的实现如下：
```c++
template <typename T>
void arrayList<T>::push_back(const T &theElement)
{
    if (listSize == arrayLength) // 如果线性表满了
        arrayLength *= 2;        // 二倍扩大线性表大小
    listSize++;
    element[listSize - 1] = theElement;
}
```

该函数只需要我们在结尾直接插入一个元素，当不需要重定义列表大小时，时间复杂度为：$O(1)$，否则，由于我们数组大小进行了翻倍，为：$O(listSize)$


## 2.(P105.T16)

`arrayList<T>::clear()`函数的实现如下：
```c++
template <typename T>
void arrayList<T>::clear()
{
    delete[] element;             // 释放原数组内存空间
    element = new T[arrayLength]; // 按数组大小重新生成空间
    listSize = 0;                 // 初始化线性表长度为0
}
```
因为清空数组的过程中，我们需要摧毁每个元素，而重新分配空间的复杂度为1，因此，该函数的时间复杂度为$O(listSize)$

最终，我们可以利用主函数来实现以上两个函数的检验，主函数如下：

```c++
int main()
{
    arrayList<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);
    arr.output(); // 正常应当输出1 2 3
    arr.clear();
    arr.push_back(4);
    arr.push_back(5);
    arr.push_back(6);
    arr.output(); // 正常应当输出4 5 6
    return 0;
}
```
经检验输出符合预测，证明程序编写正确。