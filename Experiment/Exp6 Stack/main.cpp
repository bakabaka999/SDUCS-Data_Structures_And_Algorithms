#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

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

template <class T>
class arrayStack : public Stack<T>
{
private:
    int stackTop;
    int arrayLength;
    T *stack;

public:
    arrayStack(int initialCapacity = 10);
    ~arrayStack() { delete[] stack; }
    bool empty() const { return stackTop == -1; }
    int size() const { return stackTop + 1; }
    T &top()
    {
        if (stackTop == -1)
            exit(0);
        return stack[stackTop];
    }
    void pop()
    {
        if (stackTop == -1)
            exit(0);
        stack[stackTop--].~T(); // 调用析构函数
    }
    void push(const T &theElement);
};

template <class T>
arrayStack<T>::arrayStack(int initialCapacity)
{
    if (initialCapacity < 1)
        exit(0);
    arrayLength = initialCapacity;
    stack = new T[arrayLength];
    stackTop = -1;
}

template <class T>
void arrayStack<T>::push(const T &theElement)
{
    if (stackTop == arrayLength - 1) // 栈满了，加倍空间
    {
        arrayLength *= 2;
        T *newStack = new T[arrayLength];
        for (int i = 0; i <= stackTop; i++)
            newStack[i] = stack[i];
        delete[] stack;
        stack = newStack;
    }
    // 栈顶压入元素
    stack[++stackTop] = theElement;
}

// 符号优先级比较函数
bool Priority(char a)
{
    int x, y;
    if (a == '*' || a == '/')
        return true;
    else if (a == '+' || a == '-')
        return false;
}

// 生成后缀表达式
void Suffix_Exp(const char *exp, arrayStack<char> &exp_stack)
{
    arrayStack<char> symbol_stack;
    while (*exp != '\0')
    {
        if (*exp >= '0' && *exp <= '9') // 如果对应字符为数字，直接压入后缀表达式栈
            exp_stack.push(*exp);
        else if (*exp == '(') // 如为'('，直接入符号栈
            symbol_stack.push(*exp);
        else if (*exp == ')') // 如为')'，符号位出栈至'('
        {
            while (symbol_stack.top() != '(')
            { // 不断向表达式栈压入符号
                exp_stack.push(symbol_stack.top());
                symbol_stack.pop();
            }
            symbol_stack.pop(); // 弹出'('
        }
        else // 其他运算符号
        {
            if (symbol_stack.size() == 0)
                symbol_stack.push(*exp);
            else if (Priority(*exp) > Priority(symbol_stack.top())) // 优先级更高时压入
                symbol_stack.push(*exp);
            else
            {
                while (Priority(symbol_stack.top()) >= Priority(*exp) && symbol_stack.top() != '(')
                { // 当为*/且不是(时弹出
                    exp_stack.push(symbol_stack.top());
                    symbol_stack.pop();
                    if (symbol_stack.size() == 0)
                        break;
                }
                symbol_stack.push(*exp); // 最后压入自己
            }
        }
        exp++; // 移动指针
    }
    if (symbol_stack.size() != 0)
    {
        int size = symbol_stack.size();
        for (int i = 0; i < size; i++)
        {
            exp_stack.push(symbol_stack.top());
            symbol_stack.pop();
        }
    }
}

// 计算后缀表达式
double Calculate_Exp(arrayStack<char> &exp_stack)
{
    // 反转栈
    arrayStack<char> new_exp;
    int size = exp_stack.size();
    for (int i = 0; i < size; i++)
    {
        new_exp.push(exp_stack.top());
        exp_stack.pop();
    }

    // 创建操作数栈
    arrayStack<double> num_stack;
    // 开始计算结果
    double result = 0;
    size = new_exp.size();
    for (int i = 0; i < size; i++)
    {
        if (new_exp.top() >= '0' && new_exp.top() <= '9') // 如果是数字就入栈
            num_stack.push(new_exp.top() - '0');
        else // 遇到操作符就运算
        {
            double b = num_stack.top();
            num_stack.pop();
            double a = num_stack.top();
            num_stack.pop();

            switch (new_exp.top())
            {
            case '+':
                num_stack.push(a + b);
                break;
            case '-':
                num_stack.push(a - b);
                break;
            case '*':
                num_stack.push(a * b);
                break;
            case '/':
                num_stack.push(a / b);
                break;
            }
            result = num_stack.top();
        }
        new_exp.pop();
    }
    return num_stack.top();
}

int main()
{
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        arrayStack<char> exp_stack;
        char exp[2000];
        cin >> exp;
        Suffix_Exp(exp, exp_stack);
        cout << fixed << setprecision(2) << Calculate_Exp(exp_stack) << endl;
    }
    return 0;
}
