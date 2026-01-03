#include <iostream>
#include <vector>
#include <cassert>
#include "loserTree.h"

using namespace std;

// 测试用例1：基础功能
void test_basic()
{
    cout << "\n=== 测试1：基础功能 ===" << endl;
    vector<int> elements = {5, 3, 8, 1};
    LoserTree<int> tree(elements);
    tree.printTree();

    assert(tree.getWinner() == 1);
    assert(tree.getWinnerIndex() == 4);
    cout << "测试1通过" << endl;
}

// 测试用例2：相同元素处理
void test_duplicates()
{
    cout << "\n=== 测试2：相同元素处理 ===" << endl;
    vector<int> elements = {2, 2, 2, 2};
    LoserTree<int> tree(elements);
    tree.printTree();

    assert(tree.getWinner() == 2);
    assert(tree.getWinnerIndex() == 1); // 索引最小的胜出
    cout << "测试2通过" << endl;
}

// 测试用例3：动态更新测试
void test_replay()
{
    cout << "\n=== 测试3：动态更新 ===" << endl;
    vector<int> elements = {5, 3, 8, 1};
    LoserTree<int> tree(elements);

    cout << "初始状态：" << endl;
    tree.printTree();
    assert(tree.getWinner() == 1);

    // 更新原胜者（索引4）
    tree.rePlay(4, 6);
    cout << "\n更新P4为6后：" << endl;
    tree.printTree();
    assert(tree.getWinner() == 3);
    assert(tree.getWinnerIndex() == 2);

    // 更新非胜者
    tree.rePlay(1, 2);
    cout << "\n更新P1为2后：" << endl;
    tree.printTree();
    assert(tree.getWinner() == 2);
    assert(tree.getWinnerIndex() == 1);

    cout << "测试3通过" << endl;
}

// 测试用例4：归并段耗尽测试
void test_exhausted_segment()
{
    cout << "\n=== 测试4：归并段耗尽 ===" << endl;
    vector<int> elements = {5, 3, numeric_limits<int>::max(), 1};
    LoserTree<int> tree(elements);
    tree.printTree();

    assert(tree.getWinner() == 1);
    assert(tree.getWinnerIndex() == 4);

    // 更新耗尽段
    tree.rePlay(4, numeric_limits<int>::max());
    tree.printTree();
    assert(tree.getWinner() == 3);
    assert(tree.getWinnerIndex() == 2);

    cout << "测试4通过" << endl;
}

// 测试用例5：非2的幂次测试（K=5）
void test_non_power_of_two()
{
    cout << "\n=== 测试5：非2的幂次 ===" << endl;
    LoserTree<int> tree(5); // 应扩展为8个玩家
    tree.rePlay(1, 10);
    tree.rePlay(2, 5);
    tree.rePlay(3, 8);
    tree.rePlay(4, 3);
    tree.rePlay(5, 7);
    tree.printTree();

    assert(tree.getWinner() == 3);
    assert(tree.getWinnerIndex() == 4);

    // 验证虚拟玩家（6-8号）不影响结果
    tree.rePlay(4, numeric_limits<int>::max());
    tree.printTree();
    assert(tree.getWinner() == 5);
    assert(tree.getWinnerIndex() == 2);

    cout << "测试5通过" << endl;
}

// 测试用例6：连续更新压力测试
void test_stress_updates()
{
    cout << "\n=== 测试6：连续更新 ===" << endl;
    LoserTree<int> tree(4);
    vector<int> values = {9, 2, 5, 1};

    // 初始化
    for (int i = 0; i < 4; i++)
    {
        tree.rePlay(i + 1, values[i]);
    }
    assert(tree.getWinner() == 1);
    assert(tree.getWinnerIndex() == 4);

    // 连续更新序列
    vector<pair<int, int>> updates = {
        {4, 3}, // 1 -> 3
        {2, 1}, // 2 -> 1
        {4, 4}, // 3 -> 4
        {3, 0}  // 5 -> 0
    };

    for (auto [player, val] : updates)
    {
        tree.rePlay(player, val);
        cout << "\n更新P" << player << "为" << val << "后：" << endl;
        tree.printTree();
    }

    assert(tree.getWinner() == 0);
    assert(tree.getWinnerIndex() == 3);
    cout << "测试6通过" << endl;
}

int main()
{
    test_basic();
    test_duplicates();
    test_replay();
    test_exhausted_segment();
    test_non_power_of_two();
    test_stress_updates();

    cout << "\n所有测试通过！" << endl;
    return 0;
}
