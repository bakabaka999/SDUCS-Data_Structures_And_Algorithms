#include <iostream>

using namespace std;

int value = 0; // 存储最终结果

void swap(int &a, int &b)  // 交换两个元素
{
    int mid = a;
    a = b;
    b = mid;
}

void permute(int *list, int k, int m) // 排列递归函数
{
    if (k == m) // 当确定给出一种排列时
    {
        int v = 0;                          // 初始化临时价值变量
        for (int i = 0, t = 1; i <= m; i++) // 遍历此种排列
        {
            v = v + (list[i] ^ t);
            t++; // 求出排列的价值
        }
        value = value | v; // 求或运算
    }
    else // 当排列没有给出时，利用递归函数确定排列
    {
        for (int i = k; i <= m; i++)
        {
            swap(list[k], list[i]);  // 交换位置
            permute(list, k + 1, m); // 递归
            swap(list[k], list[i]);  // 复位
        }
    }
}

int main()
{
    int n;
    int list[20];
    cin >> n;
    for (int i = 0; i < n; i++)
        cin >> list[i];      // 输入数组
    permute(list, 0, n - 1); // 求解全排列价值的或
    cout << value << endl;
    return 0;
}