// RoutePlanner.h
#ifndef ROUTEPLANNER_H
#define ROUTEPLANNER_H

#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

using namespace std;

struct Station
{
    string StationName; // 站点名称
    int StationID;      // 站点ID
    int x, y;           // 图中的坐标
};

struct Line
{
    string LineName;             // 线路名称
    int LineID;                  // 线路ID
    int Price;                   // 线路价格
    vector<Station> StationList; // 按顺序存储的站点序列
};

// 状态图哈希表使用辅助结构体
struct StateNode
{
    // 状态节点定义，下二为外码
    int StationID;
    int LineID; // 如果为-1表示未上车
    bool operator==(const StateNode &other) const
    {
        return StationID == other.StationID && LineID == other.LineID;
    }
};
// 在 std 命名空间里特化 hash<StateNode>
namespace std
{
    template <>
    struct hash<StateNode>
    {
        size_t operator()(StateNode const &s) const noexcept
        {
            // 经典的 hash combine
            return hash<int>()(s.StationID) ^ (hash<int>()(s.LineID) << 1);
        }
    };
}

class RoutePlanner
{
private:
    unordered_map<int, Station> stationMap; // 哈希表存储站点，通过ID快速查询站点
    unordered_map<int, Line> lineMap;       // 哈希表存储线路，通过ID快速查询线路

    unordered_map<string, int> stationName_to_ID; // 哈希表优化名字查询，通过名称快速查找站点ID
    unordered_map<string, int> lineName_to_ID;    // 哈希表优化名字查询，通过名称快速查找线路ID

    unordered_map<int, vector<int>> stationID_to_LineIDs; // 哈希表优化查询经过某站点所有线路ID

    int nextStationID = 0; // 站点ID计数器
    int nextLineID = 0;    // 路径ID计数器

private:
    // 三种图
    unordered_map<int, vector<pair<int, int>>> lineTransferGraph;         // 顶层图：线路反演图
    unordered_map<int, vector<pair<int, double>>> stationGraph;           // 中层图：交通站点图
    unordered_map<StateNode, vector<pair<StateNode, double>>> stateGraph; // 底层图：状态扩展图

public:
    // 公有方法
    RoutePlanner() = default;

    // 添加一条完整线路
    void addLine(const string &l_name,
                 const vector<pair<string, pair<int, int>>> &stationList,
                 int price,
                 int intervalTime,
                 double speed);

    // 查询路径
    vector<string> findCheapestPath(const string &startStationName, const string &endStationName); // Q2求解：任意两站之间最便宜的路径

    vector<string> findFastestPathWithoutWaiting(const string &startStationName, const string &endStationName); // Q3求解：任意两站之间最省通勤时间路径（不考虑等待时间）

    vector<string> findFastestPathWithWaiting(const string &startStationName, const string &endStationName); // Q4求解：任意两站之间最省通勤时间路径（考虑等待时间）

private:
    // 辅助函数
    int registerStation(const string &s_name, int x, int y); // 实际实现建站
    int registerLine(const string &l_name);
    double calDistance(const pair<int, int> &p1, const pair<int, int> &p2); // 计算欧氏距离

    // 统一求解三张图的Dijkstra
    template <class Node, class CostType>
    vector<Node> dijkstra(
        const Node &start,
        const Node &end,
        const unordered_map<Node, vector<pair<Node, CostType>>> &graph);
};

// 为了导出为动态库，添加C接口导出
#ifdef _WIN32
#define API __declspec(dllexport)
#else
#define API
#endif

typedef struct
{
    const char *name; // 站点名称
    int x;            // x坐标
    int y;            // y坐标
} CStation;

extern "C"
{
    /**
     * C 接口：查询路径
     * @param mode   "cheapest" / "fast_no_wait" / "fast_with_wait"
     * @param start  起点站名
     * @param end    终点站名
     * @return       返回一段 JSON 格式的 C 字符串，例如: {"path":["A","B","C"]}
     */
    API const char *query_route(const char *mode,
                                const char *start,
                                const char *end);
    /**
     * C 接口：添加一条线路
     * @param line_name      线路名称
     * @param stations       站点数组，每个元素包含名称和坐标
     * @param num_stations   站点数量
     * @param price         价格
     * @param interval_time 发车间隔时间
     * @param speed         行驶速度
     */
    API void add_line(const char *line_name,
                      const CStation *stations,
                      int num_stations,
                      int price,
                      int interval_time,
                      double speed);
    /**
     * C 接口：重置图
     */
    API void reset_planner();
}

#endif // ROUTEPLANNER_H
