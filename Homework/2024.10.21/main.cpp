#include "arrayQueue.h"

using namespace std;

int main()
{
    arrayQueue<int> q;
    for (int i = 1; i <= 5; i++)
        q.push(i);
    q.pop();
    q.output(); // 应当输出：2 3 4 5
    arrayQueue<int> a, b;
    q.break_down(a, b);
    a.output(); // 应当输出：2 4
    b.output(); // 应当输出：3 5
    
    return 0;
}
