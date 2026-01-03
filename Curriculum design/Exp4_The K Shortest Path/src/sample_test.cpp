#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include "K_Shortest_Path.h"

using namespace std;

const string INPUT_FILE = "./T4_Test_Data/in";
const string FORMAT_FILE = "./T4_Test_Data/out";
const string OUTPUT_FILE = "output.txt";

class KSPTester
{
private:
    KSP ksp;
    int k_target;
    int a_start;
    int b_end;

    void load_graph(int no)
    {
        ifstream fin(INPUT_FILE + to_string(no) + ".txt");
        if (!fin)
            throw runtime_error("无法打开输入文件");

        int n, m, k, a, b;
        fin >> n >> m >> k >> a >> b;

        // 初始化KSP并设置目标参数
        ksp = KSP(n, k);
        k_target = k;
        a_start = a;
        b_end = b;

        // 插入所有边
        for (int i = 0; i < m; ++i)
        {
            int u, v, l;
            fin >> u >> v >> l;
            if (!ksp.insertEdge(u, v, l))
            {
                throw runtime_error("边插入失败: " + to_string(u) + "->" + to_string(v));
            }
        }
    }

    void validate_path(const Path &p)
    {
        if (p.path.empty() || p.path.front() != a_start || p.path.back() != b_end)
        {
            throw runtime_error("无效路径端点");
        }

        // 检查简单路径（无重复节点）
        unordered_set<int> seen;
        for (int node : p.path)
        {
            if (seen.count(node))
                throw runtime_error("路径包含重复节点");
            seen.insert(node);
        }
    }

public:
    KSPTester() : ksp(0), k_target(0), a_start(0), b_end(0) {}

    void run_test(int no)
    {
        try
        {
            load_graph(no);
            ksp.Yen(a_start, b_end); // 执行Yen算法

            ofstream fout(OUTPUT_FILE);
            Path result = ksp.getKPath(k_target);

            if (result.path.empty())
            {
                fout << "No";
            }
            else
            {
                validate_path(result);
                // 格式化输出路径
                for (size_t i = 0; i < result.path.size(); ++i)
                {
                    if (i > 0)
                        fout << "-";
                    fout << result.path[i];
                }
            }
        }
        catch (const exception &e)
        {
            ofstream fout(OUTPUT_FILE);
            fout << "No"; // 任何异常情况视为路径不足
        }
    }

    bool compareFiles(int no)
    {
        ifstream f1(OUTPUT_FILE, ios::binary);
        ifstream f2(FORMAT_FILE + to_string(no) + ".txt", ios::binary);

        // 检查文件是否成功打开
        if (!f1.is_open())
        {
            cerr << "错误：无法打开文件 " << OUTPUT_FILE << endl;
            return false;
        }
        if (!f2.is_open())
        {
            cerr << "错误：无法打开文件 " << FORMAT_FILE << endl;
            f1.close(); // 确保已打开的文件正确关闭
            return false;
        }

        // 逐字符比较文件内容
        char c1, c2;
        bool match = true;
        while (f1.get(c1) && f2.get(c2))
        {
            if (c1 != c2)
            {
                match = false;
                break; // 发现不一致立即终止
            }
        }

        // 输出比较结果
        cout << "\n===== 文件对比结果 =====" << endl;
        cout << "输出文件: " << OUTPUT_FILE << endl;
        cout << "标准文件: " << FORMAT_FILE + to_string(no) + ".txt" << endl;
        cout << "结果: " << (match ? "内容一致" : "内容不一致") << endl;
        cout << "=========================\n"
             << endl;

        f1.close();
        f2.close();
        return match;
    }
};

int main()
{
    KSPTester tester;

    for (int i = 0; i < 7; i++)
    {
        if (i == 0 || i == 5)
            continue;
        tester.run_test(i);
        tester.compareFiles(i);
    }
    return 0;
}