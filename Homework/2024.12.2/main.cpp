#include <iostream>
#include <stdexcept>
using namespace std;

class AdjacencyMatrix
{
private:
    int n;   // 顶点数
    int **a; // 存储邻接矩阵的数组

public:
    // 构造函数，初始化大小为n的矩阵
    AdjacencyMatrix(int n)
    {
        this->n = n;
        a = new int *[n - 1]; // 创建n-1行
        for (int i = 0; i < n - 1; ++i)
        {
            a[i] = new int[n]; // 每行有n列
            for (int j = 0; j < n; j++)
                a[i][j] = 0;
        }
    }

    // 析构函数，释放内存
    ~AdjacencyMatrix()
    {
        for (int i = 0; i < n - 1; ++i)
        {
            delete[] a[i];
        }
        delete[] a;
    }

    // 设置矩阵元素，O(1)
    void set(int i, int j, int value)
    {
        if (i == j)
        {
            // 对角线元素不存储
            throw invalid_argument("Diagonal elements are not stored.");
        }
        if (i < j)
        {
            // 上三角部分
            a[i - 1][j - 1] = value;
        }
        else
        {
            // 下三角部分
            a[i - 2][j - 1] = value;
        }
    }

    // 获取矩阵元素，O(1)
    int get(int i, int j)
    {
        if (i == j)
        {
            // 对角线元素默认为0
            return 0;
        }
        if (i < j)
        {
            // 上三角部分
            return a[i - 1][j - 1];
        }
        else
        {
            // 下三角部分
            return a[i - 2][j - 1];
        }
    }

    // 获取顶点数
    int getSize()
    {
        return n;
    }
};

int main()
{
    // 测试代码
    AdjacencyMatrix adj(5); // 创建一个5个顶点的图

    adj.set(1, 2, 1);
    adj.set(1, 3, 1);
    adj.set(2, 3, 1);
    adj.set(3, 4, 1);
    adj.set(4, 5, 1);

    cout << "Matrix value at (1, 2): " << adj.get(1, 2) << endl;
    cout << "Matrix value at (3, 4): " << adj.get(3, 4) << endl;
    cout << "Matrix value at (2, 1): " << adj.get(2, 1) << endl; // 下三角元素，应该为0

    return 0;
}
