#include "linkedStack.h"

using namespace std;

int main()
{
    linkedStack<int> int_stack;
    int_stack.push(1);
    int_stack.push(2);

    chainNode<int>* e = new chainNode<int>(114514);
    int_stack.pushNode(e);
    cout << int_stack.popNode()->element << endl;
    cout << int_stack.top() << endl;

    return 0;
}