#include <iostream>
using namespace std;

int value[100];   // 用于存储子集内的价值
bool choose[20];  // 以此来决定是否选取元素形成子集
int solution = 0; // 最终异或和的结果


// 方法1：利用递归求解子集
int Xor(int *arr, int m, int n) // 分别为输入数组，标志，最大个数
{
    int sum = 0;         // 一个子集的价值和
    int num = 0;         // 存入价值数组的标志
    if (m >= 0 && m < n) // 当选择未完全确定时，调用递归函数确定
    {
        choose[m] = 1;      // 这一位取1的情况
        Xor(arr, m + 1, n); // 用递归确定下一位
        choose[m] = 0;      // 这一位取0的情况
        Xor(arr, m + 1, n); // 用递归确定下一位
    }

    if (m == n) // 选择确定后，进行价值异或计算
    {
        for (int i = 0; i < n; i++)
        {
            if (choose[i])
            {
                value[num] = arr[i]; // 确定选取元素
                num++;
            }
        }
        for (int j = 0; j < num; j++)
            sum += (j + 1) * value[j]; // 子集价值计算
        solution ^= sum;
    }
    return solution;
}

//方法2：利用位运算求解全部子集
int Subset_Xor(int *arr, int n)
{
    // 遍历所有可能的子集（从 1 到 (2^n)-1）
    for (int i = 1; i < (1 << n); ++i)
    {
        int sum = 0;
        int num = 0;
        // 遍历数组内每个元素，检查是否在当前子集中
        for (int j = 0; j < n; ++j)
        {
            // 如果第 j 位在 i 的二进制表示中为 1，则第 j 个元素在当前子集中
            if (i & (1 << j)) // 通过右移的j来检验这一位是否在i的二进制中为1
            {
                sum += (num + 1) * arr[j];
                num++;
            }
        }
        solution ^= sum;
    }
    return solution;
}

int main()
{
    int n;
    int arr[20];
    cin >> n;
    for (int i = 0; i < n; i++)
        cin >> arr[i];
    cout << Subset_Xor(arr, n) << endl;
    // cout << Xor(arr, 0, n) << endl;
    return 0;
}