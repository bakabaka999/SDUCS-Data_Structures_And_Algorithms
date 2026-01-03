#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include "skipList.h"

using namespace std;
using namespace chrono;

// 数据生成函数：随机生成大小为n，最大值不超过max_key的随机数序列，并存储在key中
vector<int> generateRandomKeys(int n, int max_key)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, max_key - 1);

    vector<int> keys;
    for (int i = 0; i < n; ++i)
        keys.push_back(dis(gen));
    return keys;
}

// 性能测试函数
void performanceTest()
{
    // 测试数据大小
    const int NUM_ELEMENTS = 1000000; // 数据量
    const int MAX_KEY = 2000000;      // 最大键值
    const int TEST_OPS = 100000;      // 操作次数

    skipList<int, int> list(MAX_KEY, NUM_ELEMENTS, 0.5);

    // 生成测试数据
    auto keys = generateRandomKeys(NUM_ELEMENTS, MAX_KEY);
    auto testKeys = generateRandomKeys(TEST_OPS, MAX_KEY);

    /*========== 调整前测试 ==========*/
    // 利用chrono库实现时间统计
    // 插入测试
    auto start = high_resolution_clock::now();
    for (int key : keys)
    {
        list.insert({key, 1});
    }
    auto insertTime = duration_cast<milliseconds>(high_resolution_clock::now() - start);

    // 查找测试
    start = high_resolution_clock::now();
    for (int i = 0; i < TEST_OPS; ++i)
    {
        list.find(testKeys[i]);
    }
    auto findBeforeTime = duration_cast<milliseconds>(high_resolution_clock::now() - start);

    // 删除测试
    start = high_resolution_clock::now();
    for (int i = 0; i < TEST_OPS; ++i)
    {
        list.erase(testKeys[i]);
    }
    auto eraseBeforeTime = duration_cast<milliseconds>(high_resolution_clock::now() - start);

    /*========== 结构调整 ==========*/
    start = high_resolution_clock::now();
    list.adjust();
    auto adjustTime = duration_cast<milliseconds>(high_resolution_clock::now() - start);

    /*========== 调整后测试 ==========*/
    // 查找测试
    start = high_resolution_clock::now();
    for (int i = 0; i < TEST_OPS; ++i)
    {
        list.find(testKeys[i]);
    }
    auto findAfterTime = duration_cast<milliseconds>(high_resolution_clock::now() - start);

    // 删除测试
    start = high_resolution_clock::now();
    for (int i = 0; i < TEST_OPS; ++i)
    {
        list.erase(testKeys[i]);
    }
    auto eraseAfterTime = duration_cast<milliseconds>(high_resolution_clock::now() - start);

    /*========== 结果输出 ==========*/
    cout << "================ 性能测试报告 ================\n";
    cout << "数据集规模: " << NUM_ELEMENTS << " 节点\n";
    cout << "测试操作量: " << TEST_OPS << " 次\n\n";

    cout << "[插入操作]\n";
    cout << "总耗时: " << insertTime.count() << " ms\n";
    cout << "平均耗时: " << insertTime.count() / (double)NUM_ELEMENTS << " ms/节点\n\n";

    cout << "[调整前操作]\n";
    cout << "查找总耗时: " << findBeforeTime.count() << " ms\n";
    cout << "查找平均耗时: " << findBeforeTime.count() / (double)TEST_OPS << " ms/次\n";
    cout << "删除总耗时: " << eraseBeforeTime.count() << " ms\n";
    cout << "删除平均耗时: " << eraseBeforeTime.count() / (double)TEST_OPS << " ms/次\n\n";

    cout << "[结构调整]\n";
    cout << "adjust() 总耗时: " << adjustTime.count() << " ms\n\n";

    cout << "[调整后操作]\n";
    cout << "查找总耗时: " << findAfterTime.count() << " ms\n";
    cout << "查找平均耗时: " << findAfterTime.count() / (double)TEST_OPS << " ms/次\n";
    cout << "删除总耗时: " << eraseAfterTime.count() << " ms\n";
    cout << "删除平均耗时: " << eraseAfterTime.count() / (double)TEST_OPS << " ms/次\n";
    cout << "=============================================\n";
}

int main()
{
    performanceTest();
    return 0;
}