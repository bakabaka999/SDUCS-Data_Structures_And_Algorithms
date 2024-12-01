#include <iostream>
#include <string>

using namespace std;

struct Node
{
    // 霍夫曼树的结点
    char ch;
    int freq;

    Node() : ch('\0'), freq(0) {}
    Node(char c, int f) : ch(c), freq(f) {}

    bool operator>(const Node &other) const
    {
        return freq > other.freq;
    }
    bool operator<=(const Node &other) const
    {
        return freq <= other.freq;
    }
};

template <class T>
class minHeap
{
private:
    T *heap;
    int arrayLength;
    int heapSize;

public:
    minHeap(int initialCapacity = 10); // 构造函数
    ~minHeap() { delete[] heap; }      // 析构函数

    int Size() { return heapSize; }
    void initialize(T *theHeap, int n); // 堆初始化
    void push(const T &theElement);     // 堆插入
    const T &top()
    {
        if (heapSize == 0)
            throw runtime_error("Heap is empty!");
        return heap[1];
    } // 堆顶元素读取
    T pop();                    // 堆顶元素弹出
    void heapSort(T *a, int n); // 堆排序
};

template <class T>
minHeap<T>::minHeap(int initialCapacity)
{
    arrayLength = initialCapacity + 1;
    heapSize = 0;
    heap = new T[arrayLength];
}

template <class T>
void minHeap<T>::initialize(T *theHeap, int n)
{
    // 传入新数组（不一定完成堆化）
    delete[] heap;
    heap = new T[n + 1];
    for (int i = 1; i <= n; i++)
        heap[i] = theHeap[i];
    heap[0] = T();
    arrayLength = n + 1;
    heapSize = n;

    // 堆化(从最后一个有孩子的结点开始，调整子树为小根堆)
    for (int root = heapSize / 2; root >= 1; root--)
    {
        T rootElement = heap[root]; // 找到父节点元素

        // 为父节点rootElement寻找位置
        int child = 2 * root;
        while (child <= heapSize) // 孩子不能超过堆大小
        {
            // heap[child]取兄弟中较小者
            if (child < heapSize && heap[child] > heap[child + 1])
                child++;
            // 是否可以将rootElement放到heap[root]上
            if (rootElement <= heap[child])
                break;                     // 可以
            heap[child / 2] = heap[child]; // 不可以
            child *= 2;                    // 下移孩子完成下方子树
        }
        heap[child / 2] = rootElement;
    }
}

template <class T>
void minHeap<T>::push(const T &theElement)
{ // 把theElement插到合适的地方
    // 空间不足时扩容
    if (heapSize == arrayLength - 1) // 堆满了，加倍空间
    {
        arrayLength *= 2;
        T *newHeap = new T[arrayLength];
        for (int i = 1; i <= heapSize; i++)
            newHeap[i] = heap[i];
        delete[] heap;
        heap = newHeap;
    }

    // 寻找插入位置
    // currentNode从新叶子向上移动
    int currentNode = ++heapSize;
    while (currentNode != 1 && heap[currentNode / 2] > theElement) // 该节点不是根节点且父节点元素大于插入元素
    {
        // 不能把TheElement插到这
        heap[currentNode] = heap[currentNode / 2]; // 元素向下移动
        currentNode /= 2;                          // currentNode移向双亲
    }

    heap[currentNode] = theElement;
}

template <class T>
T minHeap<T>::pop()
{
    // 如果堆为空，直接返回
    if (heapSize == 0)
        throw runtime_error("Heap is empty!");
    // 删除最后一个元素，然后重新建堆
    T minElement = heap[1]; // 保存堆顶元素以便返回
    T lastElement = heap[heapSize--];

    // 从根开始，为最后一个元素寻找位置
    int currentNode = 1,
        child = 2;            // currentNode的孩子
    while (child <= heapSize) // 建堆过程
    {
        // heap[child]是currentNode的更小的孩子
        if (child < heapSize && heap[child] > heap[child + 1])
            child++;

        // 可以把lastElement放在heap[currentNode]么
        if (lastElement <= heap[child])
            break; // 可以
        // 不可以
        heap[currentNode] = heap[child];
        currentNode = child;
        child *= 2;
    }
    heap[currentNode] = lastElement;

    return minElement;
}

template <class T>
void minHeap<T>::heapSort(T *a, int n)
{
    T *tempHeap = new T[n + 1];
    for (int i = 1; i <= n; i++)
        tempHeap[i] = a[i];
    tempHeap[0] = T();

    initialize(tempHeap, n);

    for (int i = n; i >= 1; i--)
    {
        cout << heap[1] << " ";
        pop();
    }
    cout << endl;

    delete[] tempHeap;
}

// 计算霍夫曼编码长度的函数
int huffmanLength(const string &str)
{
    // 频率统计数组
    int freq[26] = {0};
    int uniqueCount = 0; // 实际出现的字符数量
    for (char c : str)
    {
        if (freq[c - 'a'] == 0) // 如果这个字符没统计过
            uniqueCount++;
        freq[c - 'a']++;
    }

    // 使用最小堆进行霍夫曼编码
    minHeap<Node> pq;

    // 将频率与字符插入最小堆
    // 创建仅包含有效字符的节点数组
    Node *nodes = new Node[uniqueCount + 1];
    int nodeCount = 0;
    for (int i = 0; i < 26; i++)
        if (freq[i] > 0)
            nodes[++nodeCount] = Node('a' + i, freq[i]);
    pq.initialize(nodes, nodeCount);

    // 构建霍夫曼树
    int length = 0;
    while (pq.top().freq != 0 && pq.Size() > 1)
    {
        Node left = pq.pop();                        // 最小频率
        Node right = pq.pop();                       // 次小频率
        Node combined('\0', left.freq + right.freq); // 合并新节点
        pq.push(combined);                           // 插入最小堆

        length += left.freq + right.freq;
    }

    return length;
}

int main()
{
    string input;
    cin >> input;

    cout << huffmanLength(input) << endl;

    return 0;
}