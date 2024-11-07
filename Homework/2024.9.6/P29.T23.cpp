#include<iostream>

using namespace std;

int gcd(int x,int y)
{
    if(y == 0)
        return x;  // 普通部分
    else
        return gcd(y, x % y); // 递归部分
}

int main()  // 主函数用于测试
{
    int a = 20, b = 30, c = 112, d = 42;
    cout << "gcd(20,30)=" << gcd(20, 30) << endl
         << "gcd(112,42)=" << gcd(112, 42) << endl;
    return 0;
}