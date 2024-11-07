#include "chain.h"

using namespace std;

template <class T>
void meld(chain<T> &a, chain<T> &b, chain<T> &c)
{ // 把a，b两个链表的数据轮流插入c中
    // 创建三个链表的迭代器
    chain<int>::iterator ap = a.begin();
    chain<int>::iterator bp = b.begin();
    chain<int>::iterator cp = c.begin();
    while (ap != NULL || bp != NULL)
    {
        if (ap != NULL) // 先尝试从a插
        {
            c.push_back(*ap);
            ap++;
        }
        if (bp != NULL) // 再尝试从b插
        {
            c.push_back(*bp);
            bp++;
        }
    }
}

int main()
{
    string a;
    chain<int> a, b, c;
    for (int i = 0; i < 5; i++)
    {
        a.push_back(i);
        b.push_back(i + 5);
    }
    a.erase(4);
    a.erase(3);
    meld(a, b, c);
    c.output(cout);
}