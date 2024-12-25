#include <iostream>
#include <climits>

using namespace std;

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
    const bool empty() { return heapSize == 0; }

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

// 图链表的结点
struct graphNode
{
    int element;
    graphNode *next;
    int weight;

    graphNode(int e, graphNode *node, int w)
    {
        element = e;
        next = node;
        weight = w;
    }
};

// 图链表
struct graphChain
{
    graphNode *firstNode;
    graphNode *lastNode;
};

// Prim算法中的堆元素结构
struct HeapNode
{
    int node;
    int key; // 该节点的最小边权重

    HeapNode(int n = 0, int k = 0) : node(n), key(k) {}

    bool operator>(const HeapNode &other) const
    {
        if (key == other.key)
            return node > other.node; // 如果权重相同，按节点编号升序排序
        return key > other.key;       // 否则按权重升序排序
    }

    bool operator<=(const HeapNode &other) const
    {
        if (key == other.key)
            return node <= other.node; // 如果权重相同，按节点编号升序排序
        return key <= other.key;       // 否则按权重升序排序
    }
};

class linkedGraph
{
protected:
    int n;             // 节点数
    int e;             // 边数
    graphChain *aList; // 邻接表

public:
    linkedGraph(int num_of_Vertices = 0);
    ~linkedGraph() { delete[] aList; }

    // 对边的处理
    void insertEdge(int i, int j, int weight); // 插入边
    long long prim();                          // prim算法
};

int main()
{
    int n, e;
    cin >> n >> e;
    linkedGraph graph(n);
    for (int k = 0; k < e; k++)
    {
        int i, j, w;
        cin >> i >> j >> w;
        graph.insertEdge(i, j, w);
    }
    cout << graph.prim() << endl;
    return 0;
}

linkedGraph::linkedGraph(int num_of_Vertices)
{
    n = num_of_Vertices;
    e = 0;
    aList = new graphChain[n + 1]; // 第一个元素没用上
    for (int i = 0; i <= n; i++)   // 初始空间归为空
        aList[i].firstNode = nullptr;
}

void linkedGraph::insertEdge(int i, int j, int weight)
{
    // 向 i 的邻接表添加一条边
    if (aList[i].firstNode == nullptr)
    {
        aList[i].firstNode = aList[i].lastNode = new graphNode(j, nullptr, weight);
    }
    else
    {
        aList[i].lastNode->next = new graphNode(j, nullptr, weight);
        aList[i].lastNode = aList[i].lastNode->next;
    }

    // 向 j 的邻接表添加一条边（无向图）
    if (aList[j].firstNode == nullptr)
    {
        aList[j].firstNode = aList[j].lastNode = new graphNode(i, nullptr, weight);
    }
    else
    {
        aList[j].lastNode->next = new graphNode(i, nullptr, weight);
        aList[j].lastNode = aList[j].lastNode->next;
    }
}

long long linkedGraph::prim()
{
    long long totalWeight = 0;

    int *dist = new int[n + 1];      // 记录每个节点的最短边权
    bool *reached = new bool[n + 1]; // 标记每个节点是否已经加入最小生成树
    fill(dist, dist + n + 1, INT_MAX);
    fill(reached, reached + n + 1, false);

    // 起始节点距离标记为0
    dist[1] = 0;

    minHeap<HeapNode> heap(n); // 开个大小为n的最小堆+
    heap.push(HeapNode(1, 0)); // 初始推入第一个节点，权重为0

    while (!heap.empty())
    {
        HeapNode temp = heap.pop();
        int v = temp.node,
            weight = temp.key;

        if (reached[v])
            continue; // 如果到达过了就不加入了
        reached[v] = true;
        totalWeight += weight;

        // 更新相邻节点
        for (graphNode *Node = aList[v].firstNode; Node != nullptr; Node = Node->next)
        {
            int j = Node->element;
            if (!reached[j] && dist[j] > Node->weight)
            {
                dist[j] = Node->weight;
                heap.push(HeapNode(j, dist[j])); // 更新堆中的节点
            }
        }
    }

    delete[] dist;
    delete[] reached;

    return totalWeight;
}
