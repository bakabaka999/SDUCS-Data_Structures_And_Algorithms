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
