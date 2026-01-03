#include "minHeap.h"
#include <climits>
using namespace std;

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

    minHeap<HeapNode> heap(n); // 开个大小为n的最小堆
    heap.push(HeapNode(1, 0)); // 初始推入第一个节点，权重为0

    while (!heap.empty())
    {
        HeapNode temp = heap.pop();
        int v = temp.node,
            weight = temp.key;

        if (reached[v])
            continue; // 如果到达过了就不加入了，避免成环
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
