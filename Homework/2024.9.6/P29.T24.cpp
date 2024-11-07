#include <iostream>

using namespace std;

template <class T>
bool Search(T x, T *a, int n) // 传参为待查值、数组指针与元素个数
// 查找到返回真，否则返回假
{
    if (n - 1 == 0) // 如果查到头了，说明没查到
        return false;
    if (x == a[n - 1]) // 末尾如果查到了
        return true;
    else
        return Search(x, a, n - 1);
}

int main() // 测试函数编写是否正确
{
    int a[5] = {1, 2, 3, 4, 5};
    double b[5] = {1.1, 2.2, 3.3, 4.4, 5.5};
    char c[5] = {'a', 'b', 'c', 'd', 'e'};

    int x1 = 3, x2 = 8;
    double y1 = 2.2, y2 = 2.3;
    char z1 = 'c', z2 = 'x';

    cout << "int:" << Search(x1, a, 5) << "," << Search(x2, a, 5) << endl;
    cout << "double:" << Search(y1, b, 5) << "," << Search(y2, b, 5) << endl;
    cout << "char:" << Search(z1, c, 5) << "," << Search(z2, c, 5) << endl;

    return 0;
}