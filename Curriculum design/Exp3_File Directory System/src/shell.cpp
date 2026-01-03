#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <fstream>
#include "catalogTree.h"

using namespace std;

// 作用：在对象生命周期内，把 std::cout 的输出重定向到指定的输出流
class CoutRedirector
{
public:
    // 构造函数：传入一个新的输出流指针（newStream）
    CoutRedirector(ostream *newStream)
        : oldBuf_(cout.rdbuf()) // 保存当前cout的缓冲区（即原本的输出目的地，通常是控制台）
    {
        cout.rdbuf(newStream->rdbuf()); // 将cout的缓冲区切换到newStream的缓冲区，实现重定向
    }

    // 析构函数：在对象销毁时自动调用
    ~CoutRedirector()
    {
        cout.rdbuf(oldBuf_); // 恢复cout原来的缓冲区（即恢复原来的输出位置）
    }

private:
    streambuf *oldBuf_; // 成员变量：用来保存cout原本的缓冲区指针
};

class CatalogShell
{
private:
    CatalogTree tree;
    istream *input;
    ostream *output;

    vector<string> split_cmd(const string &input)
    {
        vector<string> tokens;
        string token;
        bool in_quote = false;
        char quote_char = '\0';

        for (char c : input)
        {
            if (in_quote)
            {
                if (c == quote_char)
                {
                    in_quote = false;
                    if (!token.empty())
                    {
                        tokens.push_back(token);
                        token.clear();
                    }
                }
                else
                {
                    token += c;
                }
            }
            else
            {
                if (c == '"' || c == '\'')
                {
                    in_quote = true;
                    quote_char = c;
                }
                else if (isspace(c))
                {
                    if (!token.empty())
                    {
                        tokens.push_back(token);
                        token.clear();
                    }
                }
                else
                {
                    token += c;
                }
            }
        }

        if (!token.empty())
        {
            tokens.push_back(token);
        }
        return tokens;
    }

public:
    // 默认构造函数（标准输入输出）
    CatalogShell() : input(&cin), output(&cout)
    {
    }

    // 指定输入输出流
    CatalogShell(istream *in, ostream *out) : input(in), output(out)
    {
        if (!in || !out)
            throw invalid_argument("流指针不能为空");
    }

    // 指定输出文件
    explicit CatalogShell(const string &outfile)
        : input(&cin), output(new ofstream(outfile))
    {
        if (!static_cast<ofstream *>(output)->is_open())
        {
            delete output;
            throw runtime_error("无法打开输出文件: " + outfile);
        }
    }

    // 指定输入输出文件
    CatalogShell(const string &infile, const string &outfile)
        : input(new ifstream(infile)), output(new ofstream(outfile))
    {
        if (!static_cast<ifstream *>(input)->is_open())
        {
            delete input;
            delete output;
            throw runtime_error("无法打开输入文件: " + infile);
        }
        if (!static_cast<ofstream *>(output)->is_open())
        {
            delete input;
            delete output;
            throw runtime_error("无法打开输出文件: " + outfile);
        }
    }

    ~CatalogShell()
    {
        if (input != &cin)
            delete input;
        if (output != &cout)
            delete output;
    }

    void run()
    {
        *output << "目录管理系统 (输入 help 查看命令列表)\n";

        while (true)
        {
            // 处理流结束状态
            if (input->eof())
            {
                *output << "\n检测到输入结束\n";
                break;
            }

            *output << "> ";
            string input_line;
            if (!getline(*input, input_line))
                break;

            auto args = split_cmd(input_line);
            if (args.empty())
                continue;

            try
            {
                if (args[0] == "exit" || args[0] == "quit")
                {
                    break;
                }
                else if (args[0] == "load")
                {
                    if (args.size() < 2)
                        throw runtime_error("用法: load <文件名>");
                    {
                        CoutRedirector redirect(output);
                        tree.Load("./archives/" + args[1]);
                    }
                    *output << "已从 " << args[1] << " 加载目录结构\n";
                }
                else if (args[0] == "save")
                {
                    if (args.size() < 2)
                        throw runtime_error("用法: save <文件名>");
                    {
                        CoutRedirector redirect(output);
                        tree.Save("./archives/" + args[1]);
                    }
                    *output << "已保存目录结构到 " << args[1] << endl;
                }
                else if (args[0] == "cd")
                {
                    if (args.size() == 1)
                    {
                        CoutRedirector redirect(output);
                        tree.Cd_1();
                    }
                    else
                    {
                        {
                            CoutRedirector redirect(output);
                            tree.Cd_4(args[1]);
                        }
                        *output << "当前路径已切换\n";
                    }
                }
                else if (args[0] == "mkdir")
                {
                    if (args.size() < 2)
                        throw runtime_error("用法: mkdir <目录名>");
                    {
                        CoutRedirector redirect(output);
                        tree.Mkdir(args[1]);
                    }
                    *output << "已创建目录 " << args[1] << endl;
                }
                else if (args[0] == "mkfile")
                {
                    if (args.size() < 2)
                        throw runtime_error("用法: mkfile <文件名>");
                    {
                        CoutRedirector redirect(output);
                        tree.Mkfile(args[1]);
                    }
                    *output << "已创建文件 " << args[1] << endl;
                }
                else if (args[0] == "dir")
                {
                    CoutRedirector redirect(output);
                    tree.Dir();
                }
                else if (args[0] == "delete")
                {
                    if (args.size() < 2)
                        throw runtime_error("用法: delete <名称>");
                    {
                        CoutRedirector redirect(output);
                        tree.Delete(args[1]);
                    }
                    *output << "已删除 " << args[1] << endl;
                }
                else if (args[0] == "find")
                {
                    if (args.size() < 2)
                        throw runtime_error("用法: find <名称> [file|dir]");
                    string type = "";
                    if (args.size() >= 3)
                    {
                        if (args[2] != "file" && args[2] != "dir")
                        {
                            throw runtime_error("类型参数必须是 file 或 dir");
                        }
                        type = args[2];
                    }
                    CoutRedirector redirect(output);
                    tree.Find(args[1], type);
                }
                else if (args[0] == "cp")
                {
                    bool force = false;
                    string src_path, dest_path;

                    if (args.size() >= 2 && args[1] == "-f")
                    {
                        if (args.size() < 4)
                            throw runtime_error("用法: cp [-f] <源路径> <目标路径>");
                        force = true;
                        src_path = args[2];
                        dest_path = args[3];
                    }
                    else
                    {
                        if (args.size() < 3)
                            throw runtime_error("用法: cp [-f] <源路径> <目标路径>");
                        src_path = args[1];
                        dest_path = args[2];
                    }

                    {
                        CoutRedirector redirect(output);
                        tree.Cp(src_path, dest_path, force);
                    }
                    *output << "复制成功" << endl;
                }
                else if (args[0] == "tree")
                {
                    int depth = -1;
                    if (args.size() >= 2)
                    {
                        try
                        {
                            depth = stoi(args[1]);
                            if (depth < 1 && depth != -1)
                                throw invalid_argument("");
                        }
                        catch (...)
                        {
                            throw runtime_error("无效的深度参数: " + args[1]);
                        }
                    }
                    CoutRedirector redirect(output);
                    tree.Tree(depth);
                }
                else if (args[0] == "clear")
                {
                    CoutRedirector redirect(output);
                    tree.Clear();
                }
                else if (args[0] == "help")
                {
                    *output << "命令列表:\n"
                            << "  load <文件>        加载目录结构\n"
                            << "  save <文件>        保存目录结构\n"
                            << "  cd [路径]          切换/显示当前目录\n"
                            << "  mkdir <目录>       创建新目录\n"
                            << "  mkfile <文件>      创建新文件\n"
                            << "  dir                列出当前目录内容\n"
                            << "  cp [-f] <源> <目标> 复制文件/目录\n"
                            << "  delete <名称>      删除文件或目录\n"
                            << "  tree [深度]        显示目录树结构（可选深度限制）\n"
                            << "  find <名称> [类型]  搜索文件(file)或目录(dir)\n"
                            << "  clear              清屏\n"
                            << "  exit/quit          退出系统\n";
                }
                else
                {
                    throw runtime_error("未知命令: " + args[0]);
                }
            }
            catch (const exception &e)
            {
                *output << "错误: " << e.what() << endl;
            }
        }
    }
};

int main()
{
    CatalogShell shell;
    shell.run();
    return 0;
}