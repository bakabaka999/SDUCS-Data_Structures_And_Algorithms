# 数据结构与算法作业  2024/11/22

## 1.(P399.T21)

解：对于一个图的邻接矩阵，我们可以采用邻接矩阵进行存储，而一般来说，一个图不存在自环，对于这种图，其对角线元素恒为0，那么，我们就不必要存储其对角线，只需存储其余部分。

因此，假如说我们要存储一个节点数为n的图，那么其原本的矩阵大小应当为n*n，那么这里由于我们不存储对角线，我们只需要用一个(n-1)*n的矩阵来存储就可以了。下面，通过实例来展示我们是怎么存储的：

假定一个4个节点的图的邻接矩阵是这样的，为4*4矩阵：

$$
 \left[
 \begin{matrix}
   0 & 1 & 0 & 1 \\
   1 & 0 & 1 & 0 \\
   0 & 1 & 0 & 1 \\
   1 & 0 & 1 & 0
\end{matrix}
\right]
$$


那么，抛弃掉对角线后，我们把上三角向下移动与下面拼接，就形成了新矩阵结构：

$$
\left[
\begin{matrix}
  1 & 1 & 0 & 1 \\
  0 & 1 & 1 & 0 \\
  1 & 0 & 1 & 1
\end{matrix}
\right]
$$

这样，原本的上三角矩阵(满足$i>j$)中的元素$(i,j)$，本来是存储在$a[i-1][i-1]$中，现在依然存储在这里；原本的上三角矩阵(满足$i<j$)中的元素$(i,j)$，本来是存储在$a[i-1][i-1]$中，现在应当存储在$a[i-2][i-1]$中。通过建立这样的映射，我们就压缩了矩阵占用的空间。下面通过一个$AdjacencyMatrix$类，我们实现了这种存储方式存储的邻接矩阵，并实现了$O(1)$复杂度的get和set：因为我们知道映射关系，所以我们可以通过映射关系直接定位到对应位置，这种计算自然是常数时间可以完成的。

```c++
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
```

测试主函数的输出与预期是一致的，证明了类与方法的正确性。

## 2.(P412.T41)

解：

(1) 邻接链表和邻接数组如下所示：

<img src="扫描全能王 2024-12-04 23.38.jpg" alt="alt text" width="450px">

<div style="page-break-after: always;"></div>

<img src="扫描全能王 2024-12-04 23.38_2.jpg" alt="alt text" width="450px">

<div style="page-break-after: always;"></div>


(2) 从顶点4开始的BFS序列为：

$$
[4, 3, 5, 7, 2, 6, 1]
$$

(3) 构成的子图如下图所示

<img src="扫描全能王 2024-12-04 23.38_3.jpg" alt="alt text" width="500px">

<div style="page-break-after: always;"></div>


