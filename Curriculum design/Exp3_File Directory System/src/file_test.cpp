#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include "catalogTree.h"

using namespace std;

const string INPUT_FILE = "./T3_Test_Data/in1.txt";
const string ACTUAL_OUTPUT = "actual_output.txt";
const string EXPECTED_OUTPUT = "./T3_Test_Data/out1.txt";

class AutoTester
{
    CatalogTree tree;
    ifstream input;
    ofstream output;

    vector<string> parse_command(const string &line)
    {
        vector<string> args;
        stringstream ss(line);
        string arg;
        while (ss >> arg)
            args.push_back(arg);
        return args;
    }

    void process_commands()
    {
        string line;
        while (getline(input, line))
        {
            auto args = parse_command(line);
            if (args.empty())
                continue;

            try
            {
                if (args[0] == "dir")
                    tree.Dir();
                else if (args[0] == "cd")
                {
                    if (args.size() == 1)
                        tree.Cd_1();
                    else if (args[1] == "..")
                        tree.Cd_2();
                    else if (args.size() == 2)
                        tree.Cd_4(args[1]);
                    else
                        throw runtime_error("cd参数错误");
                }
                else if (args[0] == "mkdir")
                {
                    if (args.size() != 2)
                        throw runtime_error("mkdir需要1参数");
                    tree.Mkdir(args[1]);
                }
                else if (args[0] == "mkfile")
                {
                    if (args.size() != 2)
                        throw runtime_error("mkfile需要1参数");
                    tree.Mkfile(args[1]);
                }
                else if (args[0] == "delete")
                {
                    if (args.size() != 2)
                        throw runtime_error("delete需要1参数");
                    tree.Delete(args[1]);
                }
                else if (args[0] == "save")
                {
                    if (args.size() != 2)
                        throw runtime_error("save需要1参数");
                    tree.Save("./archives/" + args[1]);
                }
                else if (args[0] == "load")
                {
                    if (args.size() != 2)
                        throw runtime_error("load需要1参数");
                    tree.Load("./archives/" + args[1]);
                }
                else if (args[0] == "quit")
                    break;
                else
                    throw runtime_error("未知命令: " + args[0]);
            }
            catch (const exception &e)
            {
                output << "错误: " << e.what() << endl;
            }
        }
    }

    void compare_files()
    {
        ifstream actual(ACTUAL_OUTPUT);
        ifstream expected(EXPECTED_OUTPUT);
        string a_line, e_line;
        int line_num = 1;
        bool all_match = true;

        cout << "===== 测试结果 =====" << endl;

        while (true)
        {
            bool a_has = !!getline(actual, a_line);
            bool e_has = !!getline(expected, e_line);

            if (!a_has && !e_has)
                break;

            if (a_has != e_has)
            {
                cout << "? 文件行数不一致（实际："
                     << line_num << "，预期：" << line_num << "）" << endl;
                all_match = false;
                break;
            }

            if (a_line != e_line)
            {
                cout << "? 第" << line_num << "行不一致" << endl;
                cout << "实际: " << a_line << endl;
                cout << "预期: " << e_line << endl;
                all_match = false;
            }

            line_num++;
        }

        if (all_match)
        {
            cout << "测试通过，输出完全匹配" << endl;
        }
        else
        {
            cout << "测试失败，存在差异" << endl;
        }
    }

public:
    AutoTester() : output(ACTUAL_OUTPUT), tree(output)
    {
        input.open(INPUT_FILE);
        if (!input)
            throw runtime_error("无法打开输入文件");
    }

    void run()
    {
        process_commands();
        output.close();
        compare_files();
    }
};

int main()
{
    try
    {
        AutoTester tester;
        tester.run();
    }
    catch (const exception &e)
    {
        cerr << "测试系统错误: " << e.what() << endl;
        return 1;
    }
    return 0;
}