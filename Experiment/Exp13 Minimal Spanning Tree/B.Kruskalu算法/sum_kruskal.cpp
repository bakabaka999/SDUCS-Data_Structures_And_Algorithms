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

int UFS[200010]; // 全局变量，并查集数组

struct Edge
{
    int i, j, w;
    bool operator<(const Edge &e) const { return w < e.w; }

    Edge(int I = 0, int J = 0, int W = 0) : i(I), j(J), w(W) {}
};

// 找到并查集的祖宗，并且进行路径压缩优化
int ufs_find(int x)
{
    if (UFS[x] != x)
        UFS[x] = ufs_find(UFS[x]);
    return UFS[x];
}

class Graph
{
protected:
    int n;                 // 节点数
    int e;                 // 边数
    arrayList<Edge> edges; // 边表

public:
    Graph(int n_num, int e_num);
    ~Graph() {}

    // 对边的处理
    void insertEdge(int i, int j, int weight); // 插入边
    long long kruskal();                       // prim算法
};

int main()
{
    int n, e;
    cin >> n >> e;
    Graph graph(n, e);
    for (int k = 0; k < e; k++)
    {
        int i, j, w;
        cin >> i >> j >> w;
        graph.insertEdge(i, j, w);
    }
    cout << graph.kruskal() << endl;
    return 0;
}

Graph::Graph(int n_num, int e_num)
{
    n = n_num;
    e = e_num;
}

void Graph::insertEdge(int i, int j, int weight)
{
    edges.push_back(Edge(i, j, weight));
}

long long Graph::kruskal()
{
    edges.arr_sort();
    long long totalWeight = 0;

    // 初始化并查集
    for (int i = 1; i <= n; i++)
        UFS[i] = i;

    for (int i = 0; i < e; i++)
    {
        int u = edges[i].i, v = edges[i].j, w = edges[i].w;

        // 判断两个端点是否在同一集合
        int root_u = ufs_find(u), root_v = ufs_find(v);
        if (root_u != root_v)
        {
            totalWeight += w;     // 选择这条边
            UFS[root_u] = root_v; // 合并两个集合
        }
    }
    return totalWeight;
}
