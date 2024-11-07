# 数据结构与算法作业  2024/9/6

## 1.(P23.T16)

解：最终编写程序如下所示：
```c++
#include <iostream>

using namespace std;

enum signType
{
    Plus,
    Minus
};

class currency
{
public:
    // 构造函数
    currency(signType theSign = Plus,
             unsigned long theDollers = 0,
             unsigned int theCents = 0);
    // 析构函数
    ~currency() {}
    // 原成员函数
    void setValue(signType, unsigned long, unsigned int);
    void setValue(double);
    signType getSign() const { return sign; }
    unsigned long getDollers() const { return dollers; }
    unsigned int getCents() const { return cents; }
    currency add(const currency &) const;
    currency &increment(const currency &);
    void output() const;

    // 新增成员函数
    void input();                        // 输入值
    currency subtract(const currency &); // 减去值
    currency percent(double);            // 百分比
    currency multiply(double);           // 乘积
    currency divide(double);             // 作商

private:
    signType sign;         // 对象的符号
    unsigned long dollers; // 美元的数量
    unsigned int cents;    // 美分的数量
};

void currency::setValue(signType theSign, unsigned long theDollers, unsigned int theCents) // 通过分别输入各部分实现输入
{
    sign = theSign;
    dollers = theDollers;
    cents = theCents;
}

void currency::setValue(double theAmout) // 通过浮点数输入
{
    if (theAmout < 0)
    {
        sign = Minus;
        theAmout = -theAmout;
    }
    else
        sign = Plus;
    dollers = (unsigned long)theAmout;
    cents = (unsigned int)((theAmout + 0.001 - dollers) * 100);
}

currency::currency(signType theSign, unsigned long theDollers, unsigned int theCents) // 构造函数实现
{
    this->setValue(theSign, theDollers, theCents);
}

currency currency::add(const currency &x) const
{
    long a1, a2, a3;
    currency result;
    // 把调用对象转化为符号整数
    a1 = dollers * 100 + cents;
    if (sign == Minus)
        a1 = -a1;

    a2 = x.dollers * 100 + x.cents;
    if (x.sign == Minus)
        a2 = -a2;

    a3 = a1 + a2;

    if (a3 < 0)
    {
        result.sign = Minus;
        a3 = -a3;
    }
    else
        result.sign = Plus;

    result.dollers = a3 / 100;
    result.cents = a3 - result.dollers * 100;

    return result;
}

currency &currency::increment(const currency &x)
{
    *this = add(x);
    return *this;
}

void currency::output() const
{
    if (sign == Minus)
        cout << "-";
    cout << '$' << dollers << '.';
    if (cents < 10)
        cout << '0';
    cout << cents;
}

void currency::input()
{
    cout << "分别输入有符号美元与美分：";
    long mid_d;
    int mid_c;
    cin >> mid_d >> mid_c;
    if (mid_d < 0)
    {
        sign = Minus;
        mid_d = -mid_d;
    }
    dollers = mid_d;
    cents = mid_c;
}

currency currency::subtract(const currency &x)
{
    long a1, a2, a3;
    currency result;
    // 把调用对象转化为符号整数
    a1 = dollers * 100 + cents;
    if (sign == Minus)
        a1 = -a1;

    a2 = x.dollers * 100 + x.cents;
    if (x.sign == Minus)
        a2 = -a2;

    a3 = a1 - a2;

    if (a3 < 0)
    {
        result.sign = Minus;
        a3 = -a3;
    }
    else
        result.sign = Plus;

    result.dollers = a3 / 100;
    result.cents = a3 - result.dollers * 100;

    return result;
}

currency currency::percent(double x)
{
    long a1, a2;
    currency result;
    // 把调用对象转化为符号整数
    a1 = dollers * 100 + cents;
    if (sign == Minus)
        a1 = -a1;

    a2 = a1 * (x + 0.001) / 100;

    if (a2 < 0)
    {
        result.sign = Minus;
        a2 = -a2;
    }
    else
        result.sign = Plus;

    result.dollers = a2 / 100;
    result.cents = a2 - result.dollers * 100;

    return result;
}

currency currency::multiply(double x)
{
    long a1, a2;
    currency result;
    // 把调用对象转化为符号整数
    a1 = dollers * 100 + cents;
    if (sign == Minus)
        a1 = -a1;

    a2 = a1 * x;

    if (a2 < 0)
    {
        result.sign = Minus;
        a2 = -a2;
    }
    else
        result.sign = Plus;

    result.dollers = a2 / 100;
    result.cents = a2 - result.dollers * 100;

    return result;
}

currency currency::divide(double x)
{
    long a1, a2;
    currency result;
    // 把调用对象转化为符号整数
    a1 = dollers * 100 + cents;
    if (sign == Minus)
        a1 = -a1;

    a2 = a1 / x;

    if (a2 < 0)
    {
        result.sign = Minus;
        a2 = -a2;
    }
    else
        result.sign = Plus;

    result.dollers = a2 / 100;
    result.cents = a2 - result.dollers * 100;

    return result;
}

int main()
{
    // 加减的检验
    currency a, b;
    a.input();
    b.input();
    (a.add(b)).output();
    cout << endl;
    (a.subtract(b)).output();
    cout << endl;

    // 乘、除、百分比的检验
    currency x;
    double y;
    x.input();
    cout << "请输入百分比:" << endl;
    cin >> y;
    (x.percent(y)).output();
    cout << endl;
    cout << "请输入因数:" << endl;
    cin >> y;
    (x.multiply(y)).output();
    cout << endl;
    cout << "请输入除数:" << endl;
    cin >> y;
    (x.divide(y)).output();
    cout << endl;

    return 0;
}
```

## 2.(P29.T23)

解：1）$gcd(20,30)=gcd(30,20)=gcd(20,10)=gcd(10,0)=10$

$~~~~~~~~~~~~$$gcd(112,42)=gcd(42,28)=gcd(28,14)=gcd(14,0)=14$

$~~~~~~$ 2) 基础部分为：$gcd(x,y)=x~,~y=0$

$~~~~~~~~~~~$递归部分为：$gcd(x,y)=gcd(y,x~mod~y)~,~y>0$

$~~~~~~~~~~~$我们在完成第$i$次递归时，设：$gcd(x_i,y_i)=gcd(y_i,x_i~mod~y_i)$

$~~~~~~~~~~~$显然，$x_i~mod~y_i<y_i~$成立，设递归部分得到结果为$gcd(x_{i+1},y_{i+1})$，那么明显：
$$
    y_{i+1}<y_i
$$

$~~~~~~~~~~~$也就是说，$y \in N$是严格递减的，又因为$y \geq 0$，因此一定存在一整数n，使得经过n次递归

$~~~~~$后，$y_n=0$，即转化为递归函数的基础部分。

$~~~~~~$ 3) 最终编写程序如下所示：
```c++
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
```

## 3.(P29.T24)

解：最终编写程序如下所示：
```c++
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
```