#ifndef K_SHORTEST_PATHS
#define K_SHORTEST_PATHS

// 如果想要求得精确的K短路解，不妨使用Yen算法
// 时间复杂度：O(K*V*(E log V))
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

using namespace std;

struct Path
{
    vector<int> path; // 路径经过点
    int cost;         // 路径总权重

    // 最小堆比较：按路径成本升序
    bool operator>(const Path &other) const
    {
        return (cost > other.cost) || (cost == other.cost && path > other.path);
    }
};

class KSP
{
private:
    vector<vector<pair<int, int>>> adj_list;                       // 权图邻接表
    priority_queue<Path, vector<Path>, greater<Path>> k_path_heap; // k短路堆
    int V, E;                                                      // 节点数，边数
    int K;                                                         // k值大小

private:
    // Yen算法辅助函数
    string pathHash(vector<int> path); // 将路径处理为字符串以实现字符串哈希
    void generateCandidate(Path curr_path,
                           priority_queue<Path, vector<Path>, greater<Path>> &candidate,
                           unordered_set<string> &visited,
                           int y); // 生成候选序列
    Path mergePath(Path before, Path devi, int i);          // 合并序列

public:
    KSP(int v, int k = 0) : V(v), E(0), K(k)
    {
        adj_list.assign(V + 1, vector<pair<int, int>>()); // 开足够大的邻接表
    }

    // 边的相关操作
    bool existEdge(int i, int j);         // 判断边是否存在
    bool insertEdge(int i, int j, int w); // 插入边
    bool deleteEdge(int i, int j);        // 删除边

    // 最短路算法(Dijkstra算法)，用于拓展k短路
    // 为了适用于Yen算法，我们需要排除边
    Path Dijkstra(int x, int y, const unordered_map<int, int> &forbidden = {});
    // 枚举多条严格最短路，按字典序取前maxCount条
    vector<Path> Dijkstra(int x, int y, int maxCount);

    // K短路算法(Yen算法)
    void Yen(int x, int y);

    // 获取第k短路
    Path getKPath(int k);
};

#endif