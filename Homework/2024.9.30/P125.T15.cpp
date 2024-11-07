#include "chain.h"

using namespace std;

int main()
{
    chain<int> ch;
    for (int i = 0; i < 5; i++)
        ch.push_back(i);
    ch.reverse();
    ch.output(cout);
    return 0;
}