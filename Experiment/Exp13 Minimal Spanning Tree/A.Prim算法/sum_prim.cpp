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
    minHeap(int initialCapacity = 10); // ���캯��
    ~minHeap() { delete[] heap; }      // ��������

    int Size() { return heapSize; }
    const bool empty() { return heapSize == 0; }

    void initialize(T *theHeap, int n); // �ѳ�ʼ��
    void push(const T &theElement);     // �Ѳ���
    const T &top()
    {
        if (heapSize == 0)
            throw runtime_error("Heap is empty!");
        return heap[1];
    } // �Ѷ�Ԫ�ض�ȡ
    T pop();                    // �Ѷ�Ԫ�ص���
    void heapSort(T *a, int n); // ������
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
    // ���������飨��һ����ɶѻ���
    delete[] heap;
    heap = new T[n + 1];
    for (int i = 1; i <= n; i++)
        heap[i] = theHeap[i];
    heap[0] = T();
    arrayLength = n + 1;
    heapSize = n;

    // �ѻ�(�����һ���к��ӵĽ�㿪ʼ����������ΪС����)
    for (int root = heapSize / 2; root >= 1; root--)
    {
        T rootElement = heap[root]; // �ҵ����ڵ�Ԫ��

        // Ϊ���ڵ�rootElementѰ��λ��
        int child = 2 * root;
        while (child <= heapSize) // ���Ӳ��ܳ����Ѵ�С
        {
            // heap[child]ȡ�ֵ��н�С��
            if (child < heapSize && heap[child] > heap[child + 1])
                child++;
            // �Ƿ���Խ�rootElement�ŵ�heap[root]��
            if (rootElement <= heap[child])
                break;                     // ����
            heap[child / 2] = heap[child]; // ������
            child *= 2;                    // ���ƺ�������·�����
        }
        heap[child / 2] = rootElement;
    }
}

template <class T>
void minHeap<T>::push(const T &theElement)
{ // ��theElement�嵽���ʵĵط�
    // �ռ䲻��ʱ����
    if (heapSize == arrayLength - 1) // �����ˣ��ӱ��ռ�
    {
        arrayLength *= 2;
        T *newHeap = new T[arrayLength];
        for (int i = 1; i <= heapSize; i++)
            newHeap[i] = heap[i];
        delete[] heap;
        heap = newHeap;
    }

    // Ѱ�Ҳ���λ��
    // currentNode����Ҷ�������ƶ�
    int currentNode = ++heapSize;
    while (currentNode != 1 && heap[currentNode / 2] > theElement) // �ýڵ㲻�Ǹ��ڵ��Ҹ��ڵ�Ԫ�ش��ڲ���Ԫ��
    {
        // ���ܰ�TheElement�嵽��
        heap[currentNode] = heap[currentNode / 2]; // Ԫ�������ƶ�
        currentNode /= 2;                          // currentNode����˫��
    }

    heap[currentNode] = theElement;
}

template <class T>
T minHeap<T>::pop()
{
    // �����Ϊ�գ�ֱ�ӷ���
    if (heapSize == 0)
        throw runtime_error("Heap is empty!");
    // ɾ�����һ��Ԫ�أ�Ȼ�����½���
    T minElement = heap[1]; // ����Ѷ�Ԫ���Ա㷵��
    T lastElement = heap[heapSize--];

    // �Ӹ���ʼ��Ϊ���һ��Ԫ��Ѱ��λ��
    int currentNode = 1,
        child = 2;            // currentNode�ĺ���
    while (child <= heapSize) // ���ѹ���
    {
        // heap[child]��currentNode�ĸ�С�ĺ���
        if (child < heapSize && heap[child] > heap[child + 1])
            child++;

        // ���԰�lastElement����heap[currentNode]ô
        if (lastElement <= heap[child])
            break; // ����
        // ������
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

// ͼ����Ľ��
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

// ͼ����
struct graphChain
{
    graphNode *firstNode;
    graphNode *lastNode;
};

// Prim�㷨�еĶ�Ԫ�ؽṹ
struct HeapNode
{
    int node;
    int key; // �ýڵ����С��Ȩ��

    HeapNode(int n = 0, int k = 0) : node(n), key(k) {}

    bool operator>(const HeapNode &other) const
    {
        if (key == other.key)
            return node > other.node; // ���Ȩ����ͬ�����ڵ�����������
        return key > other.key;       // ����Ȩ����������
    }

    bool operator<=(const HeapNode &other) const
    {
        if (key == other.key)
            return node <= other.node; // ���Ȩ����ͬ�����ڵ�����������
        return key <= other.key;       // ����Ȩ����������
    }
};

class linkedGraph
{
protected:
    int n;             // �ڵ���
    int e;             // ����
    graphChain *aList; // �ڽӱ�

public:
    linkedGraph(int num_of_Vertices = 0);
    ~linkedGraph() { delete[] aList; }

    // �ԱߵĴ���
    void insertEdge(int i, int j, int weight); // �����
    long long prim();                          // prim�㷨
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
    aList = new graphChain[n + 1]; // ��һ��Ԫ��û����
    for (int i = 0; i <= n; i++)   // ��ʼ�ռ��Ϊ��
        aList[i].firstNode = nullptr;
}

void linkedGraph::insertEdge(int i, int j, int weight)
{
    // �� i ���ڽӱ����һ����
    if (aList[i].firstNode == nullptr)
    {
        aList[i].firstNode = aList[i].lastNode = new graphNode(j, nullptr, weight);
    }
    else
    {
        aList[i].lastNode->next = new graphNode(j, nullptr, weight);
        aList[i].lastNode = aList[i].lastNode->next;
    }

    // �� j ���ڽӱ����һ���ߣ�����ͼ��
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

    int *dist = new int[n + 1];      // ��¼ÿ���ڵ����̱�Ȩ
    bool *reached = new bool[n + 1]; // ���ÿ���ڵ��Ƿ��Ѿ�������С������
    fill(dist, dist + n + 1, INT_MAX);
    fill(reached, reached + n + 1, false);

    // ��ʼ�ڵ������Ϊ0
    dist[1] = 0;

    minHeap<HeapNode> heap(n); // ������СΪn����С��+
    heap.push(HeapNode(1, 0)); // ��ʼ�����һ���ڵ㣬Ȩ��Ϊ0

    while (!heap.empty())
    {
        HeapNode temp = heap.pop();
        int v = temp.node,
            weight = temp.key;

        if (reached[v])
            continue; // ���������˾Ͳ�������
        reached[v] = true;
        totalWeight += weight;

        // �������ڽڵ�
        for (graphNode *Node = aList[v].firstNode; Node != nullptr; Node = Node->next)
        {
            int j = Node->element;
            if (!reached[j] && dist[j] > Node->weight)
            {
                dist[j] = Node->weight;
                heap.push(HeapNode(j, dist[j])); // ���¶��еĽڵ�
            }
        }
    }

    delete[] dist;
    delete[] reached;

    return totalWeight;
}
