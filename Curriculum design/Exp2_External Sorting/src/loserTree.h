#ifndef _LOSERTREE_H
#define _LOSERTREE_H

// 数据结构：最小输者树

#include <iostream>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <limits>
#include <cmath>

using namespace std;

template <class T>
class LoserTree
{
private:
    int original_players_num; // 原始玩家数
    int num_of_players;       // 填充后玩家数
    vector<T> players;        // 玩家数组
    vector<int> tree;         // 输者树（索引1~total_size-1，0存最终赢者）
    vector<int> winners;      // 赢者树，方便进行重赛

private:
    // 完全二叉树父节点索引计算
    int parent(int i) const { return i / 2; }

    // 获取玩家值（处理哨兵越界）
    T get_player_value(int idx) const
    {
        return (idx >= 1 && idx <= original_players_num) ? players[idx] : INT_MAX;
    }

    // 比较两个玩家，值小者胜；值相同则索引小者胜
    bool compare(int a, int b) const
    {
        if (players[a] < players[b])
            return true;
        if (players[a] == players[b])
            return a < b;
        return false;
    }

    // 获取某子树的当前胜者（递归查找）
    int get_subtree_winner(int node) const
    {
        if (node >= num_of_players) // 叶子节点
        {
            int player = node - num_of_players + 1; // 计算在玩家数组中的位置
            return (player <= original_players_num) ? player : -1;
        }
        else // 内部节点，胜者由维护的winners数组获取
        {
            return winners[node];
        }
    }

public:
    LoserTree(const vector<T> &elements);            // 构造函数1
    LoserTree(int k);                                // 构造函数2
    T getWinner() const { return players[tree[0]]; } // 获取赢者信息
    int getWinnerIndex() const { return tree[0]; }   // 获取赢者索引
    void rePlay(int player, T new_val);              // 当修改某玩家信息时进行树重构
    void rePlay(T new_val);                          // 赢者修改时重构

private:
    void build_tree();       // 构建输者树
    void adjust(int player); // 在某一修改发生时进行重构(关键)

public:
    void printTree() const;
};

// 时间复杂度：O(n)
template <class T>
LoserTree<T>::LoserTree(const vector<T> &elements)
{
    if (elements.empty())
        throw invalid_argument("Elements can't be empty!");

    // Step 1: 计算扩展后的玩家数（2的次幂）
    original_players_num = elements.size();
    num_of_players = 1;
    while (num_of_players < original_players_num)
        num_of_players <<= 1;

    // Step 2: 初始化玩家数组（1-based索引）
    players.resize(num_of_players + 1, numeric_limits<T>::max());
    for (int i = 1; i <= original_players_num; i++)
        players[i] = elements[i - 1];

    // Step 3: 初始化输者树数据结构
    tree.resize(num_of_players, -1);
    winners.resize(2 * num_of_players, -1);

    // Step 4: 初始化叶子节点赢者(构建赢者树)
    for (int i = 0; i < num_of_players; i++)
    {
        int player_index = i + 1;
        if (player_index > original_players_num)
            winners[num_of_players + i] = -1;
        else
            winners[num_of_players + i] = player_index;
    }

    // Step 5: 自底向上构建输者树
    build_tree();
}

// 时间复杂度：O(k)
template <class T>
LoserTree<T>::LoserTree(int k)
{
    if (k <= 0)
        throw invalid_argument("Merge paths K must be positive!");

    // Step 1: 计算扩展后的玩家数（2的次幂）
    original_players_num = k;
    num_of_players = 1;
    while (num_of_players < original_players_num)
        num_of_players <<= 1;

    // Step 2: 初始化
    players.resize(num_of_players + 1, numeric_limits<T>::max());
    tree.resize(num_of_players, -1);
    winners.resize(2 * num_of_players, -1);

    // 初始化叶子节点为哨兵
    for (int i = 0; i < num_of_players; i++)
        winners[num_of_players + i] = (i + 1 <= original_players_num) ? (i + 1) : -1;

    build_tree();
}

// 时间复杂度：O(log m)
template <class T>
void LoserTree<T>::rePlay(int player, T new_val)
{
    if (player < 1 || player > num_of_players) // 超出范围抛出错误
        throw out_of_range("Invalid player index!");
    players[player] = new_val;
    adjust(player);
}

// 时间复杂度：O(log m)
template <class T>
void LoserTree<T>::rePlay(T new_val)
{
    int win_index = tree[0];

    if (win_index == -1)
        return;
    // 输者树优化：如果新值更优，只更新最终赢者就可以了
    if (players[win_index] >= new_val)
    {
        players[win_index] = new_val;
        return; // 提前终止
    }
    players[win_index] = new_val;
    adjust(win_index);
}

// 时间复杂度：O(m)
template <class T>
void LoserTree<T>::build_tree()
{ // 类似建堆的过程
    for (int i = num_of_players - 1; i >= 1; --i)
    {
        int left = 2 * i;
        int right = 2 * i + 1;

        // 先获取左右孩子的赢家索引
        int left_winner = get_subtree_winner(left);
        int right_winner = get_subtree_winner(right);

        // 前三种都是包括非正常玩家的情况
        // 同时构建赢者树和输者树
        if (left_winner == -1 && right_winner == -1)
        {
            winners[i] = -1;
            tree[i] = -1;
        }
        else if (left_winner == -1)
        {
            winners[i] = right_winner;
            tree[i] = -1;
        }
        else if (right_winner == -1)
        {
            winners[i] = left_winner;
            tree[i] = -1;
        }
        else
        { // 比较计算赢者输者，同步更新
            if (compare(left_winner, right_winner))
            { 
                winners[i] = left_winner;
                tree[i] = right_winner;
            }
            else
            {
                winners[i] = right_winner;
                tree[i] = left_winner;
            }
        }
    }
    // 输者树最终赢者更新
    tree[0] = winners[1];
}

// 时间复杂度：O(log m)
template <class T>
void LoserTree<T>::adjust(int player)
{
    // 玩家编号有效性检查
    if (player < 1 || player > num_of_players)
        throw out_of_range("Invalid player index!");

    // 从叶子节点向上调整，玩家对应叶子的完全二叉树索引为 num_of_players + player -1
    int current = num_of_players + player - 1;
    int winner = player; // 当前路径的胜者（玩家编号）
    T winner_val = players[winner];

    // 自底向上遍历至根节点（tree[1]是根节点）
    while (current > 1)
    {
        int parent_node = current / 2;
        int brother = (current % 2 == 0) ? current + 1 : current - 1;

        int brother_winner = get_subtree_winner(brother);

        // 比较当前胜者和兄弟胜者
        int new_winner, new_loser;
        if (brother_winner == -1)
        {
            new_winner = winner;
            new_loser = -1;
        }
        else if (compare(winner, brother_winner))
        {
            new_winner = winner;
            new_loser = brother_winner;
        }
        else
        {
            new_winner = brother_winner;
            new_loser = winner;
        }

        // 更新父节点的败者和胜者
        tree[parent_node] = new_loser;
        winners[parent_node] = new_winner;

        // 向上传递胜者
        winner = new_winner;
        current = parent_node;
    }

    // 更新最终赢者（存储在tree[0]）
    tree[0] = winner;
}

template <class T>
void LoserTree<T>::printTree() const
{
    cout << "\n=============== Loser Tree Structure ===============\n";

    // 0. 打印最终赢者
    cout << "[Winner] P" << tree[0]
         << ": " << players[tree[0]] << endl;

    // 1. 计算树的总层数（不含叶子层）
    int tree_depth = 0;
    for (int nodes = num_of_players; nodes > 1; nodes >>= 1)
        tree_depth++;

    // 2. 逐层打印内部节点
    for (int level = 1; level <= tree_depth; ++level)
    {
        // 计算当前层的起始和结束索引
        int start = 1 << (level - 1);
        int end = (1 << level) - 1;
        end = min(end, num_of_players - 1); // 限制数组边界

        cout << "[Level " << level << "] ";
        for (int i = start; i <= end; ++i)
        {
            if (tree[i] == -1)
            {
                cout << "L" << i << ":max ";
            }
            else
            {
                cout << "L" << i << ":P" << tree[i]
                     << "(" << players[tree[i]] << ") ";
            }
        }
        cout << endl;
    }

    // 3. 打印叶子层玩家
    cout << "[Players] ";
    for (int i = 1; i <= num_of_players; ++i)
    {
        if (i > original_players_num)
        {
            cout << "P" << i << ":max ";
        }
        else
        {
            cout << "P" << i << ":" << players[i] << " ";
        }
    }

    cout << "\n===================================================\n";
}
#endif