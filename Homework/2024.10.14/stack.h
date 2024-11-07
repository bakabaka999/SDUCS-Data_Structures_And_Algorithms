#pragma once

template <class T>
class Stack
{
public:
    virtual ~Stack() {}
        // 抽象类的析构函数
    virtual bool empty() const = 0;
        // 当栈为空时返回true
    virtual int size() const = 0;
        // 返回栈中元素个数
    virtual T &top() = 0;
        // 返回栈顶元素的引用
    virtual void pop() = 0;
        // 删除栈顶元素
    virtual void push(const T &theElement) = 0;
        // 将元素压入栈
};