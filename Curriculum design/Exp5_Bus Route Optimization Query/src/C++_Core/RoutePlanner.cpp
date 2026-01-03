#include "RoutePlanner.h"
#include <cmath>
#include <queue>
#include <unordered_set>
#include <nlohmann/json.hpp>

using namespace std;
using nlohmann::json;

void RoutePlanner::addLine(const string &l_name, const vector<pair<string, pair<int, int>>> &stationList, int price, int intervalTime, double speed)
{
    if (lineName_to_ID.count(l_name))
        // 线路已存在，不添加
        return;
    int lid = registerLine(l_name);

    // 创建线路对象
    Line line;
    line.LineName = l_name;
    line.LineID = lid;
    line.Price = price;

    // 将站点加入线路
    for (const auto &stationInfo : stationList)
    {
        const string &sname = stationInfo.first;
        int x = stationInfo.second.first;
        int y = stationInfo.second.second;
        int sid;
        if (stationName_to_ID.count(sname))
            sid = stationName_to_ID[sname]; // 有这个站点，直接取出
        else
            sid = registerStation(sname, x, y);      // 注册这个站点
        line.StationList.push_back(stationMap[sid]); // 把这个站点压入到line序列中

        // 维护 stationToLines 辅助表
        stationID_to_LineIDs[sid].push_back(lid);
    }

    // 保存线路
    lineMap[lid] = line;

    // 1. 更新交通站点图
    for (int i = 1; i < line.StationList.size(); i++)
    {
        int u = line.StationList[i - 1].StationID, v = line.StationList[i].StationID;
        double dist = calDistance(
            {stationMap[u].x, stationMap[u].y},
            {stationMap[v].x, stationMap[v].y});
        double travelTime = dist / speed;
        // 建立双向边
        stationGraph[u].emplace_back(v, travelTime);
        stationGraph[v].emplace_back(u, travelTime);
    }

    // 2. 更新线路反演图
    // 每个站点上，所有经过该站的线路都应该两两相连
    for (const Station &st : line.StationList)
    {
        int sid = st.StationID;
        // 先收集这个站点上已有的线路
        auto &linesHere = stationID_to_LineIDs[sid];
        for (int other : linesHere)
        {
            if (other == lid)
                continue; // 同一条线路不考虑
            Line other_line = lineMap[other];
            // 双向连边，权重为相互的price
            lineTransferGraph[lid].emplace_back(other, other_line.Price);
            lineTransferGraph[other].emplace_back(lid, price);
        }
    }

    // 3. 更新状态扩展图
    // 考虑上车，乘车，下车的行为
    for (int i = 1; i < line.StationList.size(); i++)
    {
        int u = line.StationList[i - 1].StationID, v = line.StationList[i].StationID;
        // 计算该边行驶时间
        double dist = calDistance(
            {stationMap[u].x, stationMap[u].y},
            {stationMap[v].x, stationMap[v].y});
        double rideTime = dist / speed;

        StateNode su{u, lid}, sv{v, lid};
        // a) 乘车边
        stateGraph[su].emplace_back(sv, rideTime);
        stateGraph[sv].emplace_back(su, rideTime);
        // b) 下车边(cost = 0)
        stateGraph[su].emplace_back(StateNode{u, -1}, 0.0);
        stateGraph[sv].emplace_back(StateNode{v, -1}, 0.0);
        // c) 上车边 (cost = intervalTime)
        stateGraph[{u, -1}].emplace_back(su, intervalTime);
        stateGraph[{v, -1}].emplace_back(sv, intervalTime);
    }
}

vector<string> RoutePlanner::findCheapestPath(
    const string &startStationName,
    const string &endStationName)
{
    // 1. 检查起点/终点站名
    if (!stationName_to_ID.count(startStationName) ||
        !stationName_to_ID.count(endStationName))
        return {};

    int startSID = stationName_to_ID[startStationName];
    int endSID = stationName_to_ID[endStationName];

    // 2. 拿到起点和终点所在线路列表
    auto &startLines = stationID_to_LineIDs[startSID];
    auto &endLines = stationID_to_LineIDs[endSID];
    if (startLines.empty() || endLines.empty())
        return {};

    // 3. 构造“线路换乘图”加上虚拟源和虚拟汇：
    //    - 用节点 ID = -1 作为源，连向所有 startLines，权重为该线路票价
    //    - 用节点 ID = -2 作为汇，所有 endLines 连向它，权重为 0
    unordered_map<int, vector<pair<int, int>>> G = lineTransferGraph; // 复制一下图
    const int SRC = -1, DST = -2;

    G[SRC] = {};
    for (int lid : startLines)
    {
        int price = lineMap[lid].Price;
        G[SRC].emplace_back(lid, price);
    }
    for (int lid : endLines)
    {
        G[lid].emplace_back(DST, 0);
    }

    // 4. 用 dijkstra 算从 SRC 到 DST
    auto linePath = dijkstra<int, int>(SRC, DST, G);
    if (linePath.empty())
        return {};

    /// 5. 构造合法移动表（只允许在选中线路上移动）
    unordered_map<int, vector<int>> allowedMove;
    unordered_set<int> selectedLines(linePath.begin(), linePath.end());

    for (auto &[sid, lineIDs] : stationID_to_LineIDs)
    {
        for (int lid : lineIDs)
        {
            if (!selectedLines.count(lid))
                continue; // 只考虑选中线路
            Line &line = lineMap[lid];
            for (int i = 1; i < line.StationList.size(); i++)
            {
                int u = line.StationList[i - 1].StationID;
                int v = line.StationList[i].StationID;
                allowedMove[u].push_back(v);
                allowedMove[v].push_back(u); // 双向移动
            }
        }
    }

    // 6. 正式BFS
    queue<int> q;
    unordered_map<int, int> prev; // 回溯父亲
    q.push(startSID);
    prev[startSID] = -1;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        if (u == endSID)
            break;
        for (int v : allowedMove[u])
        {
            if (!prev.count(v))
            {
                prev[v] = u;
                q.push(v);
            }
        }
    }

    // 7. 回溯路径
    if (!prev.count(endSID))
        return {}; // 无法到达

    vector<int> stationPath;
    for (int u = endSID; u != -1; u = prev[u])
        stationPath.push_back(u);
    reverse(stationPath.begin(), stationPath.end());

    // 8. 转成站名路径
    vector<string> res;
    for (int sid : stationPath)
        res.push_back(stationMap[sid].StationName);
    return res;
}

vector<string> RoutePlanner::findFastestPathWithoutWaiting(const string &startStationName, const string &endStationName)
{
    // 1. 检查起点/终点站名
    if (!stationName_to_ID.count(startStationName) ||
        !stationName_to_ID.count(endStationName))
        return {};

    int startSID = stationName_to_ID[startStationName];
    int endSID = stationName_to_ID[endStationName];

    // 2. 用dijkstra直接跑最短路即可
    auto StationIDPath = dijkstra<int, double>(startSID, endSID, stationGraph);

    // 3. 把该路径转换为字符串路径
    vector<string> res;
    for (auto sid : StationIDPath)
        res.push_back(stationMap[sid].StationName);
    return res;
}

vector<string> RoutePlanner::findFastestPathWithWaiting(const string &startStationName, const string &endStationName)
{
    // 1. 检查起点/终点站名
    if (!stationName_to_ID.count(startStationName) ||
        !stationName_to_ID.count(endStationName))
        return {};

    int startSID = stationName_to_ID[startStationName];
    int endSID = stationName_to_ID[endStationName];

    // 2. 创建状态节点 (非乘车状态) 的起点和终点
    StateNode startState{startSID, -1}; // startStation的非乘车状态
    StateNode endState{endSID, -1};     // endStation的非乘车状态

    // 3. 使用 Dijkstra 算法寻找最短路径（在状态图中）
    auto statePath = dijkstra<StateNode, double>(startState, endState, stateGraph);

    if (statePath.empty())
        return {}; // 无法找到路径

    // 4. 回溯路径并剔除重复节点
    // 遍历所有状态节点，按需去重输出站点
    vector<string> res;
    int lastSID = -1; // 上一次加入 res 的 StationID
    for (auto &st : statePath)
    {
        int sid = st.StationID;
        // 只在两个条件下输出这个站点：
        // 1) 还没有输出过任何站点
        // 2) 与上一次输出的 stationID 不同
        if (sid != lastSID)
        {
            res.push_back(stationMap[sid].StationName);
            lastSID = sid;
        }
    }
    return res;
}

int RoutePlanner::registerStation(const string &s_name, int x, int y)
{
    int id = nextStationID++;
    stationMap[id] = {s_name, id, x, y};
    stationName_to_ID[s_name] = id;
    return id;
}

int RoutePlanner::registerLine(const string &l_name)
{
    int id = nextLineID++;
    lineName_to_ID[l_name] = id;
    return id;
}

double RoutePlanner::calDistance(const pair<int, int> &p1, const pair<int, int> &p2)
{
    double dx = static_cast<double>(p1.first - p2.first);
    double dy = static_cast<double>(p1.second - p2.second);
    return sqrt(dx * dx + dy * dy);
}

template <class Node, class CostType>
vector<Node> RoutePlanner::dijkstra(const Node &start, const Node &end, const unordered_map<Node, vector<pair<Node, CostType>>> &graph)
{
    // 距离映射，初始为无穷大
    unordered_map<Node, CostType> dist;
    // 前驱节点映射，用于回溯路径
    unordered_map<Node, Node> prev;
    // 最小堆：存 (当前最短距离, 节点)
    // 自定义比较器：只看 first（距离）
    using Pair = pair<CostType, Node>;
    struct Compare
    {
        bool operator()(Pair const &a, Pair const &b) const
        {
            return a.first > b.first; // 小顶堆
        }
    };
    priority_queue<Pair, vector<Pair>, Compare> pq;

    // 初始化
    dist[start] = static_cast<CostType>(0);
    pq.emplace(dist[start], start);

    while (!pq.empty())
    {
        auto [d, u] = pq.top();
        pq.pop();
        // 如果弹出的距离已大于记录的最短距离，跳过
        if (d > dist[u])
            continue;
        // 如果到达终点，结束搜索
        if (u == end)
            break;
        // 遍历所有邻居
        auto it = graph.find(u);
        if (it == graph.end())
            continue;
        for (auto const &edge : it->second)
        {
            const Node &v = edge.first;
            CostType w = edge.second;
            CostType nd = d + w;
            // 松弛操作
            if (!dist.count(v) || nd < dist[v])
            {
                dist[v] = nd;
                prev[v] = u;
                pq.emplace(nd, v);
            }
        }
    }

    // 回溯路径
    vector<Node> path;
    if (!dist.count(end))
    {
        // 无法到达
        return path;
    }
    for (Node cur = end;; cur = prev[cur])
    {
        path.push_back(cur);
        if (cur == start)
            break;
    }
    reverse(path.begin(), path.end());
    return path;
}

// 全局算法实例（也可设计成单例）
static RoutePlanner g_planner;

// C 接口实现
extern "C" API const char *query_route(const char *mode,
                                       const char *start,
                                       const char *end)
{
    vector<string> path;
    string m(mode), s(start), t(end);

    if (m == "cheapest")
    {
        path = g_planner.findCheapestPath(s, t);
    }
    else if (m == "fast_no_wait")
    {
        path = g_planner.findFastestPathWithoutWaiting(s, t);
    }
    else if (m == "fast_with_wait")
    {
        path = g_planner.findFastestPathWithWaiting(s, t);
    }
    else
    {
        // 未知模式，返回空路径
        path.clear();
    }

    // 构造 JSON
    json out;
    out["path"] = path;
    static string ret; // 注意：用 static 保证返回指针有效
    ret = out.dump();
    return ret.c_str();
}

extern "C" API void add_line(const char *line_name,
                             const CStation *stations,
                             int num_stations,
                             int price,
                             int interval_time,
                             double speed)
{
    // 将CStation数组转换为C++的stationList格式
    vector<pair<string, pair<int, int>>> stationList;
    for (int i = 0; i < num_stations; ++i)
    {
        const CStation &cs = stations[i];
        // 注意：这里假设传入的name是合法的C字符串
        stationList.emplace_back(
            string(cs.name),
            make_pair(cs.x, cs.y));
    }

    // 调用RoutePlanner的addLine方法
    g_planner.addLine(line_name, stationList, price, interval_time, speed);
}

extern "C" API void reset_planner()
{
    g_planner = RoutePlanner(); // 重新初始化
}
