#include <iostream>

using namespace std;

struct matrixTerm // 存储稀疏矩阵非零项行列与元素的结构
{
    int row;   // 行位置
    int col;   // 列位置
    int value; // 对应值
    matrixTerm &operator=(matrixTerm &x)
    {
        row = x.row;
        col = x.col;
        value = x.value;
        return *this;
    }
};

class sparseMatrix
{
private:
    int rows,             // 矩阵行数
        cols;             // 矩阵列数
    int listSize = 0,     // 非0元素个数
        arrayLength = 10; // 数组空间大小
    matrixTerm *terms;    // 非0项表

public:
    sparseMatrix(int n, int m);               // 构造函数
    ~sparseMatrix() { delete[] terms; }       // 析构函数
    sparseMatrix &operator=(sparseMatrix &a); // 赋值运算符重载

    void inputElement(int x, int y, int value); // 插入值
    void initial(int n, int m);                 // 重置矩阵操作
    void transpose();                           // 转置函数
    int add(sparseMatrix &a);                   // 加法
    int multiply(sparseMatrix &a);              // 乘法
    void output();                              // 输出矩阵
};

sparseMatrix::sparseMatrix(int n, int m)
{
    rows = n;
    cols = m;
    terms = new matrixTerm[arrayLength]; // 开辟新空间
}

sparseMatrix &sparseMatrix::operator=(sparseMatrix &a)
{
    rows = a.rows;
    cols = a.cols;
    arrayLength = a.arrayLength;
    listSize = a.listSize;
    delete[] terms; // 释放原本的空间
    terms = new matrixTerm[arrayLength];
    copy(a.terms, a.terms + listSize, terms); // 直接copy元素过去
    return *this;
}

void sparseMatrix::inputElement(int x, int y, int value)
{
    matrixTerm NewElement;
    NewElement.row = x;
    NewElement.col = y;
    NewElement.value = value;
    if (listSize == arrayLength)
    {                     // 如果线性表满了
        arrayLength *= 2; // 二倍扩大线性表大小
        matrixTerm *newTerms = new matrixTerm[arrayLength];
        for (int i = 0; i < listSize; ++i)
        {
            newTerms[i] = terms[i];
        }
        delete[] terms; // 释放旧数组
        terms = newTerms;
    }
    terms[listSize++] = NewElement; // 尾插入
}

void sparseMatrix::initial(int n, int m)
{
    rows = n;
    cols = m;
    listSize = 0;
    for (int i = 1; i <= rows; i++)
    { // 按行优先顺序输入矩阵的各个元素
        for (int j = 1; j <= cols; j++)
        {
            int v;
            cin >> v;
            if (v != 0) // 只有非0时传入
                inputElement(i, j, v);
        }
    }
}

void sparseMatrix::transpose()
{
    // 初始化中间矩阵大小
    sparseMatrix t(cols, rows);
    t.listSize = this->listSize;
    delete[] t.terms;
    t.terms = new matrixTerm[this->arrayLength];
    t.arrayLength = this->arrayLength;

    // 初始化数组实现转置
    int *colSize = new int[cols + 1];
    int *rowNext = new int[cols + 1];

    // 计算带转置矩阵中每一列非0元素的个数
    for (int i = 1; i <= cols; i++)
        colSize[i] = 0; // 初始化
    for (int i = 0; i < listSize; i++)
        colSize[terms[i].col]++;

    // 确定矩阵第i列第1个非0元素的对应索引
    rowNext[1] = 0;
    for (int i = 2; i <= cols; i++)
        rowNext[i] = rowNext[i - 1] + colSize[i - 1];

    // 实现*this到b的转置复制
    for (int i = 0; i < listSize; i++)
    {
        int j = rowNext[terms[i].col]++; // j是该位置元素对应列第一个元素在新矩阵中的索引，由于我们采取行主映射存储，这实际就是该元素的索引，取完以后，我们++索引，来取到该列的下一个元素

        // 行列转置
        t.terms[j].row = this->terms[i].col;
        t.terms[j].col = this->terms[i].row;
        t.terms[j].value = this->terms[i].value;
    }
    *this = t; // 将新生成矩阵作为此矩阵

    // 释放数组空间
    delete[] colSize;
    delete[] rowNext;
}

int sparseMatrix::add(sparseMatrix &a)
{
    if (rows != a.rows || cols != a.cols) // 无法相加
    {
        *this = a;
        return -1;
    }
    else // 进行相加操作
    {
        sparseMatrix result(rows, cols); // 创建结果矩阵

        int it = 0, ib = 0;                        // 两个矩阵数组的索引
        while (it != listSize && ib != a.listSize) // 分别遍历两个数组
        {
            // 利用对应元素数据计算在二维数组中的索引
            int tIndex = (terms[it].row) * cols + terms[it].col;
            int bIndex = (a.terms[ib].row) * cols + a.terms[ib].col;

            if (tIndex < bIndex) // 如果第一个元素在前
            {
                result.inputElement(terms[it].row, terms[it].col, terms[it].value);
                it++; // 元素索引前进
            }
            else if (tIndex > bIndex) // 如果第二个元素在前
            {
                result.inputElement(a.terms[ib].row, a.terms[ib].col, a.terms[ib].value);
                ib++; // 元素索引前进
            }
            else // 如果两个元素在同一个位置
            {
                if (terms[it].value + a.terms[ib].value != 0) // 和不为0时加入稀疏矩阵
                {
                    result.inputElement(terms[it].row, terms[it].col, terms[it].value + a.terms[ib].value);
                }
                it++, ib++;
            }
        }

        // 复制剩余项
        for (; it != listSize; it++)
            result.inputElement(terms[it].row, terms[it].col, terms[it].value);
        for (; ib != a.listSize; ib++)
            result.inputElement(a.terms[ib].row, a.terms[ib].col, a.terms[ib].value);

        *this = result;
        return 0;
    }
}

int sparseMatrix::multiply(sparseMatrix &a)
{
    if (cols != a.rows)
    {
        *this = a;
        return -1;
    }
    else
    {
        sparseMatrix C(rows, a.cols);
        int rowSize[10000] = {0}; // 每行非零元素的个数
        int colNext[10000] = {0}; // 每行第一个非零元素对应的索引
        int Answer[10000] = {0};  // 存放乘法结果

        for (int i = 0; i < a.listSize; i++)
        {
            rowSize[a.terms[i].row]++;
        }

        colNext[1] = 0;
        for (int i = 2; i <= a.rows; i++)
            colNext[i] = colNext[i - 1] + rowSize[i - 1];

        int p = 0;
        for (int i = 1; i <= rows; i++)
        {
            fill(Answer, Answer + a.cols + 1, 0); // 清零
            while (p < listSize && terms[p].row == i)
            {
                int t = terms[p].col;
                for (int q = colNext[t]; q < colNext[t] + rowSize[t]; q++)
                {
                    Answer[a.terms[q].col] += terms[p].value * a.terms[q].value;
                }
                p++;
            }
            for (int k = 1; k <= a.cols; k++)
            {
                if (Answer[k] != 0)
                {
                    C.inputElement(i, k, Answer[k]); // 确保使用 inputElement 插入结果
                }
            }
        }
        *this = C;
        return 0;
    }
}

void sparseMatrix::output()
{
    int i, j, k = 0;
    cout << rows << " " << cols << endl;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            if (k < listSize && terms[k].row == i + 1 && terms[k].col == j + 1) // 当该元素有值时
            {
                cout << terms[k].value << " ";
                k++;
            }
            else
                cout << "0 ";
        }
        cout << endl;
    }
}

int main()
{
    sparseMatrix P(0, 0);
    int n;
    int judge;
    int row, col;
    int count;
    int oper;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        cin >> oper;
        switch (oper)
        {
        case 1:
        {
            cin >> row >> col;
            P.initial(row, col);
            break;
        }

        case 2:
        {
            cin >> row >> col;
            cin >> count;
            sparseMatrix Q1(row, col);
            for (int i = 0; i < count; i++)
            {
                int x, y, value;
                cin >> x >> y >> value;
                Q1.inputElement(x, y, value);
            }
            judge = P.multiply(Q1);
            if (judge == -1)
                cout << judge << endl;
            break;
        }

        case 3:
        {
            cin >> row >> col;
            cin >> count;
            sparseMatrix Q2(row, col);
            for (int i = 0; i < count; i++)
            {
                int x, y, value;
                cin >> x >> y >> value;
                Q2.inputElement(x, y, value);
            }
            judge = P.add(Q2);
            if (judge == -1)
                cout << judge << endl;
            break;
        }

        case 4:
        {
            P.output();
            break;
        }

        case 5:
        {
            P.transpose();
            break;
        }
        }
    }

    return 0;
}