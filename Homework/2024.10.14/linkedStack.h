#pragma once

#include <bits/stdc++.h>
#include "chain.h"
#include "stack.h"

template <class T>
class linkedStack : public Stack<T>
{
private:
    chainNode<T> *stackTop; // 栈顶指针
    int stackSize;          // 栈内元素个数
public:
    linkedStack(int initialCapacity = 10)
    {
        stackTop = NULL;
        stackSize = 0;
    }
    ~linkedStack();
    bool empty() const { return stackSize == 0; }
    int size() const { return stackSize; }
    T &top()
    {
        if (stackSize == 0)
            exit(0);
        return stackTop->element;
    }
    void pop();
    void push(const T &theElement)
    {
        stackTop = new chainNode<T>(theElement, stackTop);
        stackSize++;
    }

    // 作业函数
    void pushNode(chainNode<T>* theNode)
    {
        theNode->next = stackTop;
        stackTop = theNode;
    }
    chainNode<T>* popNode()
    {
        chainNode<T> *returnNode = stackTop;
        stackTop = stackTop->next;
        return returnNode;
    }

};

template <class T>
linkedStack<T>::~linkedStack()
{
    while (stackTop != NULL)
    { // 删除栈顶结点
        chainNode<T> *nextNode = stackTop->next;
        delete stackTop;
        stackTop = nextNode;
    }
}

template <class T>
void linkedStack<T>::pop()
{
    if (stackTop == 0)
        exit(0);

    chainNode<T> *nextNode = stackTop->next;
    delete stackTop;
    stackTop = nextNode;
    stackSize--;
}
