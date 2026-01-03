#include "K_Shortest_Path.h"
#include <unordered_set>
#include <functional>

string KSP::pathHash(vector<int> path)
{
    string str;
    for (int n : path)
        str += to_string(n) + '|';
    return str;
}

// 生成候选序列
void KSP::generateCandidate(Path curr_path,
                            priority_queue<Path, vector<Path>, greater<Path>> &candidate,
                            unordered_set<string> &visited,
                            int y)
{
    for (int i = 0; i + 1 < (int)curr_path.path.size(); ++i)
    {
        int u = curr_path.path[i], v = curr_path.path[i + 1];
        unordered_map<int, int> forbid{{u, v}};
        Path devi = Dijkstra(u, y, forbid);
        if (devi.path.empty())
            continue;
        Path np = mergePath(curr_path, devi, i);
        string h = pathHash(np.path);
        if (!visited.count(h))
        {
            visited.insert(h);
            candidate.push(np);
        }
    }
}

Path KSP::mergePath(Path before, Path devi, int i)
{
    Path merged;
    merged.path.assign(before.path.begin(), before.path.begin() + i + 1);
    // 注意：最后一个和第一个节点的重复
    if (!devi.path.empty() && devi.path[0] == merged.path.back())
    {
        merged.path.insert(merged.path.end(), devi.path.begin() + 1, devi.path.end());
    }
    else
    {
        merged.path.insert(merged.path.end(), devi.path.begin(), devi.path.end());
    }

    merged.cost = 0;
    for (int j = 0; j < i; ++j)
    { // 注意是i条边
        int u = before.path[j];
        int v = before.path[j + 1];
        for (auto [vertex, weight] : adj_list[u])
        {
            if (vertex == v)
            {
                merged.cost += weight;
                break;
            }
        }
    }
    merged.cost += devi.cost; // 加上偏离路径成本

    return merged;
}

bool KSP::existEdge(int i, int j)
{
    if (i < 1 || i > V || j < 1 || j > V)
        return false; // 范围检查
    auto &list = adj_list[i];
    for (const auto &edge : adj_list[i])
        // 使用 const引用 避免拷贝
        if (edge.first == j)
            return true;
    return false;
}

bool KSP::insertEdge(int i, int j, int w)
{
    if (i < 1 || i > V || j < 1 || j > V)
        return false; // 范围检查
    if (existEdge(i, j))
        return false;
    auto &list = adj_list[i];
    list.emplace_back(j, w);
    return true;
}

bool KSP::deleteEdge(int i, int j)
{
    if (i < 1 || i > V || j < 1 || j > V)
        return false; // 范围检查
    auto &list = adj_list[i];
    for (auto it = list.begin(); it != list.end(); it++)
    {
        if (it->first == j)
        {
            list.erase(it);
            return true;
        }
    }
    return false;
}

// 单条最短路 Dijkstra，用于spur阶段
Path KSP::Dijkstra(int x, int y, const unordered_map<int, int> &forbidden)
{
    vector<int> dist(V + 1, INT_MAX);
    dist[x] = 0;
    using State = tuple<int, int, vector<int>>;
    priority_queue<State, vector<State>, greater<State>> heap;
    heap.emplace(0, x, vector<int>{x});
    unordered_map<int, vector<int>> best_path;

    while (!heap.empty())
    {
        auto [d, u, path] = heap.top();
        heap.pop();
        if (u == y)
            return {path, d};
        if (d > dist[u])
            continue;
        for (auto [v, w] : adj_list[u])
        {
            if (forbidden.count(u) && forbidden.at(u) == v)
                continue;
            int nd = d + w;
            vector<int> new_path = path;
            new_path.push_back(v);
            if (nd < dist[v] || (nd == dist[v] && new_path < best_path[v]))
            {
                dist[v] = nd;
                best_path[v] = new_path;
                heap.emplace(nd, v, new_path);
            }
        }
    }
    return Path();
}

// 枚举多条严格等价的最短路
vector<Path> KSP::Dijkstra(int x, int y, int maxCount)
{
    // 1. 计算所有最短距离和前驱
    vector<int> dist(V + 1, INT_MAX);
    vector<vector<int>> pred(V + 1);
    dist[x] = 0;
    using PII = pair<int, int>;
    priority_queue<PII, vector<PII>, greater<PII>> heap;
    heap.emplace(0, x);

    while (!heap.empty())
    {
        auto [d, u] = heap.top();
        heap.pop();
        if (d > dist[u])
            continue;
        for (auto [v, w] : adj_list[u])
        {
            int nd = d + w;
            if (nd < dist[v])
            {
                dist[v] = nd;
                pred[v].clear();
                pred[v].push_back(u);
                heap.emplace(nd, v);
            }
            else if (nd == dist[v])
            {
                pred[v].push_back(u);
            }
        }
    }
    // 2. 排序前驱列表以保证字典序
    for (auto &prs : pred)
        sort(prs.begin(), prs.end());

    // 3. 回溯枚举路径
    vector<Path> result;
    vector<int> tmp;
    function<void(int)> dfs = [&](int u)
    {
        tmp.push_back(u);
        if (u == x)
        {
            vector<int> p = tmp;
            reverse(p.begin(), p.end());

            // 重新计算路径上的真实代价
            int cost = 0;
            for (int i = 0; i + 1 < p.size(); i++)
            {
                int u = p[i], v = p[i + 1];
                for (auto [to, w] : adj_list[u])
                {
                    if (to == v)
                    {
                        cost += w;
                        break;
                    }
                }
            }

            result.push_back({p, cost});
        }
        else
        {
            for (int pr : pred[u])
            {
                dfs(pr);
            }
        }
        tmp.pop_back();
    };
    dfs(y);

    // 统一按path字典序排序
    sort(result.begin(), result.end(), [](const Path &A, const Path &B)
         {
    if (A.cost != B.cost)
        return A.cost < B.cost;
    return A.path < B.path; });

    // 只取前maxCount条（如果需要）
    if (result.size() > maxCount)
        result.resize(maxCount);

    return result;
}

void KSP::Yen(int x, int y)
{
    // 清空已有结果
    while (!k_path_heap.empty())
        k_path_heap.pop();
    priority_queue<Path, vector<Path>, greater<Path>> candidate;
    unordered_set<string> visited;

    // 1) 初始化：多条严格等价最短路
    auto initList = Dijkstra(x, y, K);
    for (auto &p : initList)
    {
        candidate.push(p);
        visited.insert(pathHash(p.path));
    }

    // 2) spur扩展直到得到K条路径
    while (!candidate.empty() && (int)k_path_heap.size() < K)
    {
        Path best = candidate.top();
        candidate.pop();
        k_path_heap.push(best);
        generateCandidate(best, candidate, visited, y);
    }
}

Path KSP::getKPath(int k)
{
    Path p;
    auto h = k_path_heap;
    for (int i = 0; i < k - 1; i++)
    {
        if (h.empty())
            return Path();
        h.pop();
    }
    return h.top();
}
