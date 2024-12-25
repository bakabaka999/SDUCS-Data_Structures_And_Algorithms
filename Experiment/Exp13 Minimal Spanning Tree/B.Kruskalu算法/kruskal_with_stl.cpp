
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

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
    int n;              // 节点数
    int e;              // 边数
    vector<Edge> edges; // 边表

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
    sort(edges.begin(), edges.end());
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
