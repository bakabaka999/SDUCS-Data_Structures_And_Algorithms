#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "skipList.h"
using namespace std;

// 去除字符串右侧空白符
string rtrim(const string &s)
{
    size_t end = s.find_last_not_of(" \t\n\r");
    return (end == string::npos) ? "" : s.substr(0, end + 1);
}

//------------------------------ 测试程序 ------------------------------
int computeXOR(skipList<int, int> &sk)
{ // 计算键值的异或和
    int xor_sum = 0;
    for (auto it = sk.begin(); it != sk.end(); ++it)
    {
        xor_sum ^= it->first;
    }
    return xor_sum;
}

void runTests()
{ // 正式测试程序，先看看操作过程中有没有错误，然后给出得到输出
    ifstream infile("./T1_Test_Data/input_3.txt"); // 文件输入
    if (!infile)
    {
        cerr << "错误：无法打开输入文件" << endl;
        return;
    }

    ofstream outfile("output.txt"); // 文件输出
    if (!outfile)
    {
        cerr << "错误：无法创建输出文件" << endl;
        return;
    }

    int M, N;
    infile >> M >> N;

    const int largeKey = 0x7fffffff;
    skipList<int, int> sk(largeKey, max(N, 1), 0.5);

    // 初始化跳表
    for (int i = 0; i < N; ++i)
    {
        // cerr是用来输出错误信息的标准IO流
        int num;
        if (!(infile >> num))
        {
            cerr << "错误：输入文件格式错误" << endl;
            return;
        }
        sk.insert({num, num}); // 利用insert初始化
    }

    // 处理操作
    for (int i = 0; i < M; ++i)
    {
        int op;
        if (!(infile >> op))
        {
            cerr << "错误：操作次数不足" << endl;
            return;
        }

        switch (op)
        {
        case 1:
        { // 查找
            int num;
            if (!(infile >> num))
            {
                cerr << "错误：查找操作参数缺失" << endl;
                return;
            }
            outfile << (sk.find(num) ? "YES" : "NO") << endl;
            break;
        }
        case 2:
        { // 插入
            int num;
            if (!(infile >> num))
            {
                cerr << "错误：插入操作参数缺失" << endl;
                return;
            }
            sk.insert({num, num});
            outfile << computeXOR(sk) << endl;
            break;
        }
        case 3:
        { // 删除
            int num;
            if (!(infile >> num))
            {
                cerr << "错误：删除操作参数缺失" << endl;
                return;
            }
            sk.erase(num);
            outfile << computeXOR(sk) << endl;
            break;
        }
        case 4:
        { // 删除最小
            if (sk.size() > 0)
            {
                int min_val = sk.getHeaderNode()->next[0]->element.first;
                sk.erase_min();
                outfile << min_val << endl;
            }
            break;
        }
        case 5:
        { // 删除最大
            if (sk.size() > 0)
            {
                skipNode<int, int> *current = sk.getHeaderNode()->next[0];
                while (current->next[0] != sk.getTailNode())
                {
                    current = current->next[0];
                }
                int max_val = current->element.first;
                sk.erase(max_val);
                outfile << max_val << endl;
            }
            break;
        }
        default:
            cerr << "错误：未知操作类型" << op << endl;
        }
    }
}

//------------------------------ 文件对比 ------------------------------
struct DiffInfo
{ // 用于对比的结构体，存储这是哪行，这行的两个对比字符串
    int line;
    string userLine;
    string stdLine;
};

void compareFiles(const string &userFile, const string &stdFile)
{
    ifstream userStream(userFile); // 打开我们的输出文件
    if (!userStream)
    {
        cerr << "\033[31m错误：无法打开用户文件 " << userFile << "\033[0m" << endl;
        return;
    }

    ifstream stdStream(stdFile); // 打开标准输出文件
    if (!stdStream)
    {
        cerr << "\033[31m错误：无法打开标准文件 " << stdFile << "\033[0m" << endl;
        return;
    }

    vector<DiffInfo> diffs; //存储差异结构体，用于输出
    int lineNum = 1;
    bool hasContentDiff = false;
    bool hasLineCountDiff = false;
    string uLine, sLine;

    // 精确比较每一行
    while (true)
    {
        string uLine, sLine;
        bool userRead = static_cast<bool>(getline(userStream, uLine)); // 读取并检测是否读取成功
        bool stdRead = static_cast<bool>(getline(stdStream, sLine));

        // 去除行尾空白
        uLine = rtrim(uLine);
        sLine = rtrim(sLine);

        // 结束条件
        if (!userRead && !stdRead)
            break; // 全都读完了

        // 行数差异标记
        if (userRead != stdRead) // 一个读完了但另一个没有
            hasLineCountDiff = true;

        // 内容差异处理
        if (userRead || stdRead)
        {
            if (uLine != sLine)
            { // 在同一个结构体里两个不一样
                diffs.push_back({lineNum, uLine, sLine});
                hasContentDiff = true;
            }
        }

        lineNum++;
    }

    // 检查剩余内容
    while (getline(userStream, uLine))
    {
        uLine = rtrim(uLine);
        diffs.push_back({lineNum++, uLine, ""});
        hasContentDiff = hasLineCountDiff = true;
    }
    while (getline(stdStream, sLine))
    {
        sLine = rtrim(sLine);
        diffs.push_back({lineNum++, "", sLine});
        hasContentDiff = hasLineCountDiff = true;
    }

    // 输出结果
    if (!hasContentDiff && !hasLineCountDiff)
    {
        cout << "\033[32m√ 测试通过，输出完全一致\033[0m" << endl;
        return;
    }

    cout << "\033[31m× 发现 " << diffs.size() << " 处差异";
    if (hasLineCountDiff)
        cout << " (存在行数差异)";
    cout << "\033[0m" << endl;

    for (const auto &diff : diffs)
    {
        cout << "行 " << diff.line
             << "\n  用户输出: \033[31m" << diff.userLine
             << "\033[0m\n  标准答案: \033[32m" << diff.stdLine
             << "\033[0m" << endl;
    }
}

//------------------------------ 主控程序 ------------------------------
int main()
{
    cout << "正在运行测试程序..." << endl;
    runTests();
    cout << "测试完成，输出已保存到output.txt" << endl;
    compareFiles("output.txt", "./T1_Test_Data/output_3.txt");
    return 0;
}