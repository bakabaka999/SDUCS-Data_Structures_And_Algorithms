#include "K_Shortest_Path.h"
#include <iostream>

using namespace std;

int main()
{
    // 7个节点（1~7），要求生成7条最短路径
    KSP finder(7, 7);

    // 添加边（包含环、多权重分支）
    finder.insertEdge(1, 2, 3); // 1 -> 2 (3)
    finder.insertEdge(1, 3, 2); // 1 -> 3 (2)
    finder.insertEdge(2, 3, 1); // 2 -> 3 (1)
    finder.insertEdge(2, 4, 4); // 2 -> 4 (4)
    finder.insertEdge(3, 4, 2); // 3 -> 4 (2)
    finder.insertEdge(3, 5, 3); // 3 -> 5 (3)
    finder.insertEdge(4, 5, 1); // 4 -> 5 (1)
    finder.insertEdge(4, 6, 5); // 4 -> 6 (5)
    finder.insertEdge(5, 6, 2); // 5 -> 6 (2)
    finder.insertEdge(5, 7, 4); // 5 -> 7 (4)
    finder.insertEdge(6, 7, 3); // 6 -> 7 (3)
    // 添加反向边（形成环）
    finder.insertEdge(4, 2, 1); // 4 -> 2 (1)
    finder.insertEdge(5, 3, 2); // 5 -> 3 (2)

    // 计算从1到7的前7条最短路径
    finder.Yen(1, 7);

    // 输出结果
    for (int k = 1; k <= 7; k++)
    {
        Path p = finder.getKPath(k);
        if (p.path.empty())
        {
            cout << "Path " << k << ": Not Found" << endl;
            continue;
        }
        cout << "Path " << k << " (Cost=" << p.cost << "): ";
        for (int node : p.path)
        {
            cout << node << "->";
        }
        cout << "end" << endl;
    }

    return 0;
}