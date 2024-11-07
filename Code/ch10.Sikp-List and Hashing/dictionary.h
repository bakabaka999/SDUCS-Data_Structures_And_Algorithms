#pragma once

#include<bits/stdc++.h>

using namespace std;

// 创建字典模板抽象类————由形如(k,v)的数对所组成的集合
// 其中k是关键字，v是与关键字k对应的值
// 任意两个数对关键字不相等

template<class K, class E>
class dictionary
{
public:
    virtual ~dictionary() {}
                // 字典类的析构函数
    virtual bool empty() const = 0;
                // 当字典为空时返回true
    virtual int size() const = 0;
                // 返回字典中数对的数目
    virtual pair<const K, E> *find(const K &) const = 0;
                // 返回匹配数对的指针
    virtual void erase(const K &) = 0;
                // 删除匹配的数对
    virtual void insert(const pair<const K, E> &) = 0;
                // 向字典中插入一个数对
};
