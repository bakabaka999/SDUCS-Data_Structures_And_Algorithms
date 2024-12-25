#include "arrayList.h"
#include "chain.h"
#include "arrayQueue.h"
// 这里我们引入了之前构建的模板类，这里由于篇幅原因不再进行展示
// 具体类可以见与以往的实验报告中

using namespace std;

// 以上都是模板类，下面才是图论实验的重头戏

template <class T>
class graphChain : public chain<T>
{
public:
    T *eraseElement(int theVertex)
    {
        int index = this->indexOf(theVertex);
        return this->erase(index);
    }

    void sortList()
    {
        arrayList<T> temp;
        // 遍历链表，将元素加入到 tempList 中
        for (chainNode<T> *node = this->firstNode; node != NULL; node = node->next)
            temp.push_back(node->element);
        temp.arr_sort();
        this->firstNode = NULL;
        for (int i = 0; i < temp.size(); i++)
            this->push_back(temp[i]);
    }
};

class linkedGraph
{
protected:
    graphChain<int> *aList; // 邻接表
    int n;                  // 节点数
    int e;                  // 边数
public:
    linkedGraph(int num_of_Vertices = 0);
    ~linkedGraph() { delete[] aList; }

    // 对边的处理
    bool existEdge(int i, int j);  // 判断边是否存在
    void insertEdge(int i, int j); // 插入边
    void eraseEdge(int i, int j);  // 删除边
    void sortAllLists()
    {
        for (int i = 1; i <= n; i++)
            aList[i].sortList();
    }

    // 搜索函数：bfs和dfs
    int bfs(int start, arrayList<int> &result, arrayList<int> &reach);  // 广度优先搜索，得到字典序最小序列和到达点标记，返回序列长度
    void dfs(int start, arrayList<int> &result, arrayList<int> &reach); // 深度优先搜索，得到字典序最小序列和到达点标记

    // 连通分支相关
    int countConnectedComponents(arrayList<int> &minNodes); // 计算图中连通分量的个数，同时得到最小节点序列

    // 最短路径相关
    int getShortestPath(int start, int end);
};

int main()
{
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    // 图的创建
    linkedGraph graph(n);
    for (int i = 0; i < m; i++)
    {
        int op, u, v;
        cin >> op >> u >> v;
        if (op == 0)
            graph.insertEdge(u, v);
        else if (op == 1)
            graph.eraseEdge(u, v);
        // graph.output();
    }

    graph.sortAllLists();

    // 输出连通分量个数和最小点编号
    arrayList<int> minNodes;
    cout << graph.countConnectedComponents(minNodes) << endl;
    cout << minNodes << endl;
    // 输出dfs长度和序列
    arrayList<int> result;
    arrayList<int> reach;
    reach.reset(n + 1, 0);
    graph.dfs(s, result, reach);
    cout << result.size() << endl;
    cout << result << endl;
    // 输出bfs长度和序列
    result.clear();
    reach.reset(n + 1, 0);
    cout << graph.bfs(t, result, reach) << endl;
    cout << result << endl;
    // 寻找最短路径
    cout << graph.getShortestPath(s, t) << endl;
    return 0;
}

linkedGraph::linkedGraph(int num_of_Vertices)
{

    if (num_of_Vertices < 0)
        exit(0);
    n = num_of_Vertices;
    e = 0;
    aList = new graphChain<int>[n + 1]; // 第一个元素没用上
}

bool linkedGraph::existEdge(int i, int j)
{
    if (i > n || j > n || i < 1 || j < 1)
        return false;
    graphChain<int> *V = &aList[i];
    int flag = V->indexOf(j);
    if (flag == -1)
        return false;
    return true;
}

void linkedGraph::insertEdge(int i, int j)
{
    bool flag = existEdge(i, j);
    if (!flag)
    {
        aList[i].push_back(j);
        aList[j].push_back(i);
        e++;
    }
}

void linkedGraph::eraseEdge(int i, int j)
{
    bool flag = existEdge(i, j);
    if (flag)
    {
        aList[i].eraseElement(j);
        aList[j].eraseElement(i);
        e--;
    }
}

int linkedGraph::bfs(int start, arrayList<int> &result, arrayList<int> &reach)
{
    arrayQueue<int> q(10);
    reach[start] = 1;
    q.push(start);

    while (!q.empty())
    {
        // 从队列中删除一个有标记的顶点
        int w = q.front();
        q.pop();

        // 访问节点加入结果数组
        result.push_back(w);

        // 遍历 w 的邻接节点
        for (chain<int>::iterator i = aList[w].begin(); i != aList[w].end(); i++)
        {
            if (reach[*i] == 0) // 未访问的邻接节点
            {
                q.push(*i);
                reach[*i] = 1; // 标记为到达
            }
        }
    }

    return result.size();
}

void linkedGraph::dfs(int start, arrayList<int> &result, arrayList<int> &reach)
{
    // 访问当前节点，加入结果数组并标记为已访问
    reach[start] = 1;
    result.push_back(start);

    // 遍历邻接节点
    for (chain<int>::iterator i = aList[start].begin(); i != aList[start].end(); i++)
    {
        int next = *i;
        if (reach[next] == 0) // 如果未访问
        {
            dfs(next, result, reach); // 递归深度优先遍历
        }
    }
}

// 计算连通分量的数量
int linkedGraph::countConnectedComponents(arrayList<int> &minNodes)
{
    arrayList<int> reach(n + 1);
    arrayList<int> result;
    reach.reset(n + 1, 0);
    int componentCount = 0; // 连通分量计数

    // 遍历每个节点，进行DFS或BFS
    for (int i = 1; i <= n; i++)
    {
        if (reach[i] == 0)
        {
            // 以每个未访问的节点为起点，开始DFS
            result.clear();
            dfs(i, result, reach);
            minNodes.push_back(i);
            componentCount++;
        }
    }
    return componentCount;
}

// 最短路径可以用BFS来找
int linkedGraph::getShortestPath(int start, int end)
{
    if (start == end)
        return 0;

    arrayQueue<int> q(10);          // 队列用于 BFS
    arrayList<int> reach(n + 1);    // 记录节点是否已访问
    arrayList<int> distince(n + 1); // 记录每个节点的路径
    reach.reset(n + 1, 0);
    distince.reset(n + 1, 0);

    q.push(start);
    reach[start] = 1; // 标记起始节点已访问
    distince[start] = 0;

    while (!q.empty())
    {
        int current = q.front(); // 当前节点
        q.pop();

        // 遍历当前节点的邻接节点
        arrayList<int> neighbors;
        for (chain<int>::iterator i = aList[current].begin(); i != aList[current].end(); i++)
            neighbors.push_back(*i);

        // 遍历邻接节点
        for (int i = 0; i < neighbors.size(); i++)
        {
            int neighbor = neighbors[i];
            if (reach[neighbor] == 0)
            { // 如果节点没有被访问过
                q.push(neighbor);
                reach[neighbor] = 1;                        // 标记该节点已访问
                distince[neighbor] = distince[current] + 1; // 子节点距离为父节点+1

                // 如果找到了目标节点，返回
                if (neighbor == end)
                    return distince[neighbor];
            }
        }
    }

    return -1; // 如果没有找到路径
}
