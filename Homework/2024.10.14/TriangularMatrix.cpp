#include <iostream>

using namespace std;

// 下三角矩阵类
class LowerTriangularMatrix
{
private:
    double *data; // 一维数组存储
    int n;        // 矩阵维度
public:
    LowerTriangularMatrix(int size) // 构造函数
    {
        n = size;
        data = new double[n * (n + 1) / 2]; // 动态开辟数组空间
    }
    ~LowerTriangularMatrix() // 析构函数
    {
        delete[] data;
    }

    // 设置元素值
    void set(int i, int j, double value)
    {
        if (i >= j) // 因为是下三角矩阵
            data[i * (i + 1) / 2 + j] = value;
    }
    // 获取元素值
    double get(int i, int j)
    {
        return (i >= j) ? data[i * (i + 1) / 2 + j] : 0;
    }
    int size()
    {
        return n;
    }
};

// 上三角矩阵类
class UpperTriangularMatrix
{
private:
    double *data; // 一维数组存储
    int n;        // 矩阵维度
public:
    UpperTriangularMatrix(int size) // 构造函数
    {
        n = size;
        data = new double[n * (n + 1) / 2]; // 动态开辟数组空间
    }
    ~UpperTriangularMatrix() // 析构函数
    {
        delete[] data;
    }

    // 设置元素值
    void set(int i, int j, double value)
    {
        if (i <= j) // 因为是上三角矩阵
            data[i * n - i * (i - 1) / 2 + (j - i)] = value;
    }
    // 获取元素值
    double get(int i, int j)
    {
        return (i <= j) ? data[i * n - i * (i - 1) / 2 + (j - i)] : 0;
    }
    int size()
    {
        return n;
    }
};

// 普通矩阵类
class Matrix
{
private:
    double **data; // 二维指针存储二维数组
    int n;         // 矩阵维度
public:
    // 构造函数
    Matrix(int size)
    {
        n = size;
        data = new double *[n];
        for (int i = 0; i < n; i++)
        {
            data[i] = new double[n];
            for (int j = 0; j < n; j++)
                data[i][j] = 0; // 初始化为0
        }
    }
    // 析构函数
    ~Matrix()
    {
        for (int i = 0; i < n; i++)
            delete[] data[i];
        delete[] data;
    }

    // 设置元素值
    void set(int i, int j, int value)
    {
        data[i][j] = value;
    }
    // 获取元素值
    double get(int i, int j)
    {
        return data[i][j];
    }
    // 获取大小
    int size()
    {
        return n;
    }
    // 输出矩阵
    void output()
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                cout << data[i][j] << " ";
            cout << endl;
        }
    }
};

// 乘法运算函数
void multiply(LowerTriangularMatrix &low, UpperTriangularMatrix up, Matrix &m)
{
    int n = low.size();
    for (int i = 0; i < n; i++) // 遍历下三角矩阵的行
    {
        for (int j = 0; j < n; j++) // 遍历上三角矩阵的列
        {
            int sum_result = 0;
            for (int k = 0; k <= i; k++)
                sum_result += low.get(i, k) * up.get(k, j);
            m.set(i, j, sum_result);
        }
    }
}

int main()
{
    int n = 3;

    // 下三角矩阵
    LowerTriangularMatrix lower(n);
    lower.set(0, 0, 1);
    lower.set(1, 0, 2);
    lower.set(1, 1, 3);
    lower.set(2, 0, 4);
    lower.set(2, 1, 5);
    lower.set(2, 2, 6);

    // 上三角矩阵
    UpperTriangularMatrix upper(n);
    upper.set(0, 0, 7);
    upper.set(0, 1, 8);
    upper.set(0, 2, 9);
    upper.set(1, 1, 10);
    upper.set(1, 2, 11);
    upper.set(2, 2, 12);

    // 计算乘积
    Matrix matrix(n);
    multiply(lower, upper, matrix);

    // 打印结果
    matrix.output();

    return 0;
}