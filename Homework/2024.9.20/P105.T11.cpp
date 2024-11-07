#include <bits/stdc++.h>

template <typename T>
// 抽象类
class linearList
{
public:
    virtual ~linearList() {};
    virtual bool empty() const = 0;
    // 返回true，当线性表为空时
    virtual int size() const = 0;
    // 返回线性表元素个数
    virtual T &get(int theIndex) const = 0;
    // 返回索引为theIndex的元素
    virtual int indexOf(const T &theElement) const = 0;
    // 返回元素theElement第一次出现时的元素
    virtual void erase(int theIndex) = 0;
    // 删除索引为theIndex的元素
    virtual void insert(int theIndex, const T &theElement) = 0;
    // 在 theIndex位置插入元素theElement
    virtual void output() const = 0;
};

template <class T>
class arrayList : public linearList<T>
{
public:
    // 构造函数
    arrayList(int initialCapacity = 10);
    arrayList(const arrayList<T> &);
    ~arrayList() { delete[] element; }
    bool empty() const { return listSize == 0; }
    int size() const { return listSize; }
    T &get(int theIndex) const;
    int indexOf(const T &theElement) const;
    void erase(int theIndex);
    void insert(int theIndex, const T &theElement);
    void output() const;
    int capacity() const { return arrayLength; }

    // 作业新增函数
    void push_back(const T &theElement);
    void clear();

protected:
    void checkIndex(int theIndex) const;
    T *element;      // 存储线性表元素的一位数组
    int arrayLength; // 一维数组的容量
    int listSize;    // 线性表元素的个数
};

template <typename T>
arrayList<T>::arrayList(int initialCapacity)
{ // 构造函数
    if (initialCapacity < 1)
    {
        std::ostringstream s;
        s << "Initial capacity = " << initialCapacity << " Must be > 0"; // 把各种格式的数据赋值给输出流s
        std::cout << s.str() << std::endl;
    }
    arrayLength = initialCapacity;
    element = new T[arrayLength];
    listSize = 0;
}

template <typename T>
arrayList<T>::arrayList(const arrayList<T> &theList)
{ // 复制构造函数
    arrayLength = theList.arrayLength;
    listSize = theList.listSize;
    element = new T[arrayLength];
    // std::cout << theList.element << std::endl;
    // std::cout << arrayLength << std::endl;
    std::copy(theList.element, theList.element + listSize, element);
}

template <typename T>
void arrayList<T>::checkIndex(int theIndex) const
{
    if (theIndex < 0 || theIndex > listSize)
    {
        std::ostringstream s;
        s << "index = " << theIndex << "size = " << listSize;
        std::cout << s.str() << std::endl;
        exit(0);
    }
}

template <typename T>
T &arrayList<T>::get(int theIndex) const
{ // 若存在返回theIndex的元素
  // 若不存在返回输出异常
    checkIndex(theIndex);
    return element[theIndex];
}

template <typename T>
int arrayList<T>::indexOf(const T &theElement) const
{
    int theIndex = (int)(std::find(element, element + listSize, theElement) - element);
    if (theIndex == listSize)
        // 没有找到
        return -1;
    else
        return theIndex;
}

template <typename T>
void arrayList<T>::erase(int theIndex)
{
    checkIndex(theIndex);

    std::copy(element + theIndex + 1, element + listSize, element + theIndex);

    element[--listSize].~T();
}

template <typename T>
void changeLength1D(T *a, int oldLength, int newLength)
{
    if (newLength < 0)
        ;
    // throw illegalParameterValue("new length must be >= 0");

    T *temp = new T[newLength];
    int number = std::min(newLength, oldLength);
    std::copy(a, a + number, temp);
    a = temp;
    delete[] temp;
}

template <typename T>
void arrayList<T>::insert(int theIndex, const T &theElement)
{
    checkIndex(theIndex);

    if (listSize == arrayLength)
    { // 数组已满，则将数组扩展为两倍大小
        changeLength1D(element, arrayLength, 2 * arrayLength);
        arrayLength *= 2;
    }
    // 把theIndex之后的元素都右移一个位置
    std::copy_backward(element + theIndex, element + listSize, element + listSize + 1);
    element[theIndex] = theElement;
    listSize++;
}

template <typename T>
void arrayList<T>::output() const
{
    for (int i = 0; i < listSize; i++)
    {
        std::cout << *(element + i) << " ";
    }
    std::cout << std::endl;
}

template <typename T>
void arrayList<T>::push_back(const T &theElement)
{
    if (listSize == arrayLength)
    {                                       // 如果线性表满了
        arrayLength *= 2;                   // 二倍扩大线性表大小
        T *newElement = new T[arrayLength]; // 扩大后要重新分配空间
        for (int i = 0; i < listSize; ++i)
        {
            newElement[i] = element[i];
        }
        delete[] element; // 释放旧数组
        element = newElement;
    }
    element[listSize++] = theElement; // 尾插入
}

template <typename T>
void arrayList<T>::clear()
{
    delete[] element;             // 释放原数组内存空间
    element = new T[arrayLength]; // 按数组大小重新生成空间
    listSize = 0;                 // 初始化线性表长度为0
}

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