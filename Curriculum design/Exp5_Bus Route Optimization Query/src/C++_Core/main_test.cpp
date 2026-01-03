#include <iostream>
#include "RoutePlanner.h"

int main()
{
    reset_planner();

    // 添加线路L1：低价慢速，发车间隔长
    CStation l1_stations[] = {
        {"S1", 0, 0},
        {"S2", 0, 10},
        {"S3", 0, 20},
        {"S4", 0, 30}};
    add_line("L1", l1_stations, 4, 1, 10, 10.0);

    // 添加线路L2：高价快速，发车间隔较长
    CStation l2_stations[] = {
        {"S1", 0, 0},
        {"S5", 0, 15},
        {"S4", 0, 30}};
    add_line("L2", l2_stations, 3, 5, 15, 30.0);

    // 添加线路L3：中价中速，发车间隔短
    CStation l3_stations[] = {
        {"S1", 0, 0},
        {"S6", 10, 0},
        {"S7", 10, 10}};
    add_line("L3", l3_stations, 3, 2, 2, 10.0);

    // 添加线路L4：中价中速，连接换乘点
    CStation l4_stations[] = {
        {"S7", 10, 10},
        {"S9", 0, 20},
        {"S4", 0, 30}};
    add_line("L4", l4_stations, 3, 3, 3, 10.0);

    // 查询三种路径
    const char *cheapest = query_route("cheapest", "S1", "S4");
    std::cout << "最便宜路径: " << cheapest << std::endl;

    const char *fast_no_wait = query_route("fast_no_wait", "S1", "S4");
    std::cout << "不考虑等待的最快路径: " << fast_no_wait << std::endl;

    const char *fast_with_wait = query_route("fast_with_wait", "S1", "S4");
    std::cout << "考虑等待的最快路径: " << fast_with_wait << std::endl;

    return 0;
}