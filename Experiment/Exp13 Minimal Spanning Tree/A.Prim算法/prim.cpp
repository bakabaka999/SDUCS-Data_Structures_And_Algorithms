#include "minHeap.h"
#include <climits>
using namespace std;

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
