#include <iostream>
using namespace std;

template <class T>
class Sort
{
private:
    int size; // 数组大小
    T *arr;   // 指向待排序数组的指针

public:
    Sort(T *p, int size_num); // 构造函数
    ~Sort() { delete[] arr; } // 析构函数
    void Swap(T &a, T &b);    // 交换函数
    void Count_Sort();        // 计数排序
    void Select_Sort();       // 选择排序
    void Bubble_Sort();       // 冒泡排序
    void Insert_Sort();       // 插入排序
    void Output();
};

template <class T>
Sort<T>::Sort(T *p, int size_num)
{
    arr = p;
    size = size_num;
}

template <class T>
void Sort<T>::Swap(T &a, T &b)
{
    T mid;
    mid = a;
    a = b;
    b = mid;
}

template <class T>
void Sort<T>::Count_Sort()
{
    int r[size] = {0}; // 初始化名次数组
    for (int i = 0; i < size; i++)
        for (int j = 0; j < i; j++)
            if (arr[j] <= arr[i])
                r[i]++; // 如果有一个更高的，名次+1
            else
                r[j]++;
    for (int i = 0; i < size; i++)
        // 把正确的元素移动到a[i]
        while (r[i] != i)
        {
            int t = r[i];
            Swap(arr[i], arr[t]);
            Swap(r[i], r[t]);
        }
}

template <class T>
void Sort<T>::Select_Sort()
{
    bool sorted = false;
    for (int i = size; !sorted && i > 1; i--)
    {
        int k = 0;
        sorted = true;
        for (int j = 1; j < i; j++)
            if (arr[j] > arr[k])
                k = j;
            else
                sorted = false;
        Swap(arr[k], arr[i - 1]); // 选择子序列中的最大值移到子序列最后
    }
}

template <class T>
void Sort<T>::Bubble_Sort()
{
    bool sorted = false;
    for (int i = size; !sorted && i > 1; i--)
    {
        sorted = true;
        for (int j = 0; j < i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                Swap(arr[j], arr[j + 1]);
                sorted = false;
            }
        }
    }
}

template <class T>
void Sort<T>::Insert_Sort()
{
    for (int i = 1; i < size; i++)
    { // 把a[i]插到前i-1个元素构成的有序数组中
        T t = arr[i];
        int j;
        for (j = i - 1; j >= 0 && t < arr[j]; j--) // 从后往前检查插入（防止重复元素）
            arr[j + 1] = arr[j];                   // 大的元素都往后移一位
        arr[j + 1] = t;                            // 正式插入
    }
}

template <class T>
void Sort<T>::Output()
{
    for (int i = 0; i < size; i++)
        cout << arr[i] << ' ';
    cout << endl;
}

int main()
{
    int n;
    cin >> n;
    int *arr = new int[n];
    for (int i = 0; i < n; i++)
        cin >> arr[i];
    Sort<int> sort(arr, n);
    sort.Bubble_Sort();
    sort.Output();
    return 0;
}