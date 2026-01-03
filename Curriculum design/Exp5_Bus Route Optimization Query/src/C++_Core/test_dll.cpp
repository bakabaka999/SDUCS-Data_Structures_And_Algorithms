// test_complex.cpp
#include <iostream>
#include <vector>
#include <windows.h>

// 与DLL完全一致的C接口定义
typedef struct
{
    const char *name;
    int x;
    int y;
} CStation;

// 函数指针类型
typedef void (*AddLineFunc)(const char *, const CStation *, int, int, int, double);
typedef const char *(*QueryRouteFunc)(const char *, const char *, const char *);
typedef void (*ResetPlannerFunc)(); // 新增重置接口

void printResult(const char *mode, const char *start, const char *end, const char *result)
{
    std::cout << "[" << mode << "] " << start << " -> " << end << ":\n";
    if (result)
    {
        std::cout << "  " << result << "\n";
    }
    else
    {
        std::cout << "  No path found!\n";
    }
    std::cout << "-------------------------\n";
}

int main()
{
    HMODULE dll = LoadLibraryA("RoutePlanner.dll");
    if (!dll)
    {
        std::cerr << "Error loading DLL: " << GetLastError() << std::endl;
        return 1;
    }

    // 获取函数指针
    auto add_line = reinterpret_cast<AddLineFunc>(GetProcAddress(dll, "add_line"));
    auto query_route = reinterpret_cast<QueryRouteFunc>(GetProcAddress(dll, "query_route"));
    auto reset_planner = reinterpret_cast<ResetPlannerFunc>(GetProcAddress(dll, "reset_planner"));

    if (!add_line || !query_route || !reset_planner)
    {
        std::cerr << "Failed to get functions!" << std::endl;
        FreeLibrary(dll);
        return 1;
    }

    /*********************** 测试场景 ************************
     * 线路图：
     *
     * Line1 (票价5元, 发车间隔5分钟, 速度10单位/分钟)
     *   A(0,0) -> B(10,0) -> C(20,0) -> D(30,0)
     *
     * Line2 (票价3元, 发车间隔8分钟, 速度5单位/分钟)
     *   E(0,10) -> B(10,0) -> F(20,10)
     *
     * Line3 (票价8元, 发车间隔3分钟, 速度20单位/分钟)
     *   A(0,0) -> G(15,15) -> D(30,0)
     *
     * 关键点：
     * - B站是Line1和Line2的换乘站
     * - A和D在Line1和Line3都有站点
     *********************************************************/

    // 重置规划器（确保测试独立）
    reset_planner();

    // 添加 Line1
    std::vector<CStation> line1_stations = {
        {"A", 0, 0},
        {"B", 10, 0},
        {"C", 20, 0},
        {"D", 30, 0}};
    add_line("Line1", line1_stations.data(), line1_stations.size(), 5, 5, 10.0);

    // 添加 Line2
    std::vector<CStation> line2_stations = {
        {"E", 0, 10},
        {"B", 10, 0}, // 与Line1相交于B
        {"F", 20, 10}};
    add_line("Line2", line2_stations.data(), line2_stations.size(), 3, 8, 5.0);

    // 添加 Line3
    std::vector<CStation> line3_stations = {
        {"A", 0, 0},
        {"G", 15, 15},
        {"D", 30, 0} // 与Line1的D站坐标相同
    };
    add_line("Line3", line3_stations.data(), line3_stations.size(), 8, 3, 20.0);

    // 测试用例
    const char *test_cases[][3] = {
        {"cheapest", "A", "D"},      // 预期：Line1直达（5元）
        {"cheapest", "E", "F"},      // 预期：Line2直达（3元）
        {"cheapest", "E", "D"},      // 预期：E->B(Line2) 换乘 B->D(Line1)，总价5+3=8元?
        {"fast_no_wait", "A", "D"},  // 预期：Line3更快（距离更短+速度更高）
        {"fast_with_wait", "A", "D"} // 需计算等待时间的影响
    };

    // 执行所有测试
    for (auto &[mode, start, end] : test_cases)
    {
        const char *result = query_route(mode, start, end);
        printResult(mode, start, end, result);
    }

    FreeLibrary(dll);
    return 0;
}