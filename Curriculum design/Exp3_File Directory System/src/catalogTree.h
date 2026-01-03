#ifndef _CATALOG_TREE
#define _CATALOG_TREE

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Node // 目录树节点，表示一个文件夹
{
    string name;              // 文件夹名
    vector<string> file_list; // 文件夹内的文件
    vector<Node *> next;      // 文件夹的子目录
    Node *father;             // 文件夹的上一级目录

    Node(string n, Node *f = NULL)
    {
        name = n;
        father = f;
    }
};

// 目录树节点按照其名字字典序比较
inline bool cmp(const Node *x, const Node *y)
{
    return x->name < y->name;
}

class CatalogTree
{
private:
    Node *root;      // 根节点
    Node *current;   // 当前所在节点
    ostream *output; // 用于输出操作结果（可选到控制台、文件或字符串）
    bool if_output;  // 标记是否需要释放输出流内存

private:                                                    // 辅助函数(1)
    void save_tree(ostream &os, Node *node, string indent); // 递归的保存树
    void print_tree(Node *node,
                    int current_depth,
                    int max_depth,
                    const string &prefix,
                    bool is_last) const;                           // 递归的打印树
    vector<string> split_path(const string &path);                 // 分割路径
    Node *traverse_path(Node *start, const vector<string> &parts); // 路径导航核心逻辑
    void find_recursive(Node *node,                                // 当前遍历节点
                        const string &target,                 // 查找目标
                        const string &current_path,           // 当前路径
                        vector<string> &results,         // 结果集
                        const string &type);                       // 查找类型限制

private:                                                                   // 辅助函数(2)_针对cp指令
    Node *find_parent(const string &path, string &name);                   // 解析给定路径，找到其父目录节点，并返回该节点的指针
    void copy_file(Node *src_dir, const string &filename, Node *dest_dir); // 将源目录下的一个文件复制到目标目录中
    void copy_directory(Node *src, Node *dest, bool force);                // 递归复制整个目录结构

public:
    // 构造函数序列
    // 构造方法1：输出到控制台
    CatalogTree() : output(&cout), if_output(false)
    {
        root = new Node("");
        current = root;
    }
    // 构造方法2：输出到指定文件
    CatalogTree(const string &output_file_name) : if_output(true)
    {
        output = new ofstream(output_file_name);
        if (!static_cast<ofstream *>(output)->is_open())
            throw runtime_error("无法打开输出文件");
        root = new Node("");
        current = root;
    }
    // 构造方法3：输出到任意流
    CatalogTree(ostream &os) : output(&os), if_output(false)
    {
        root = new Node("");
        current = root;
    }

    // 析构函数
    ~CatalogTree()
    {
        if (if_output)
            delete output;
        delete root;
        delete current;
    }

    // 核心功能
    void Dir();                     // 列出当前目录下的所有子目录与文件项
    void Cd_1();                    // 列出当前目录的绝对路经
    void Cd_2();                    // 当前目录变为当前目录的父目录
    void Cd_3(const string &str);   // 当前目录变为 str 所表示路径的目录
    void Mkdir(const string &str);  // 在(当前目录下)创建一个子目录(名为 str)，若存在则不进行任何操作
    void Mkfile(const string &str); // 在(当前目录下)创建一个文件(名为 str) ，若存在则不进行任何操作
    void Delete(const string &str); // 删除(当前目录下)名为 str 的目录或文件，若不存在则不进行任何操作
    void Save(const string &str);   // 将从根节点开始的目录树结构保存到文件(名为 str)中
    void Load(const string &str);   // 从文件 str 中读取之前保存的目录树结构，并根据其重新建立当前目录树

    // 新增指令功能
    void Cd_4(const string &str);                                         // 按照绝对路径/相对路径进入目录
    void Tree(int depth = -1);                                            // 打印目录树（类似save，DFS遍搜树即可）（增加递归深度参数）
    void Clear();                                                         // 清屏指令
    void Find(const string &name, const string &type = "");               // 查找指令
    void Cp(const string &src_path, const string &dest_path, bool force); // 复制指令
};

#endif