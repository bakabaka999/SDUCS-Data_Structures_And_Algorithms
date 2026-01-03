#include "catalogTree.h"
#include <algorithm>
#include <functional>

/**
 * @param os    输出流
 * @param node  当前处理的节点
 * @param indent 当前缩进字符串
 */
void CatalogTree::save_tree(ostream &os, Node *node, string indent)
{ // 本质上是对树的DFS存储
    if (node != root)
    { // 根节点不保存名称
        os << indent << node->name << endl;
    }

    // 根节点文件无缩进，子目录文件缩进+4
    string file_indent = (node == root) ? "" : indent + "    ";
    for (const string &file : node->file_list)
        os << file_indent << file << "*" << endl;

    // 递归子目录
    for (Node *dir : node->next)
        save_tree(os, dir, (node == root) ? "" : indent + "    ");
}

void CatalogTree::print_tree(Node *node,
                             int current_depth,
                             int max_depth,
                             const string &prefix,
                             bool is_last) const
{
    if (!node || (max_depth != -1 && current_depth > max_depth))
    {
        return;
    }

    // 当前节点显示
    *output << prefix;
    *output << (is_last ? "└── " : "├── ") << node->name << "/";

    // 文件列表显示（先显示文件）
    const string file_connector = (node->next.empty() ? "└── " : "├── ");
    const int file_count = node->file_list.size();

    for (int i = 0; i < file_count; ++i)
    {
        const bool is_last_file = (i == file_count - 1);
        *output << endl
                << prefix
                << (is_last ? "    " : "│   ")
                << (is_last_file ? "└── " : "├── ")
                << node->file_list[i];
    }

    *output << endl;

    // 递归处理子目录
    const int child_count = node->next.size();
    for (int i = 0; i < child_count; ++i)
    {
        bool is_last_child = (i == child_count - 1);
        string new_prefix = prefix + (is_last ? "    " : "│   ");

        print_tree(node->next[i],
                   current_depth + 1,
                   max_depth,
                   new_prefix,
                   is_last_child);
    }
}

vector<string> CatalogTree::split_path(const string &path)
{
    vector<string> parts;
    string part;
    bool is_absolute = !path.empty() && path[0] == '/';

    for (char c : path)
    {
        if (c == '/')
        {
            if (!part.empty())
            {
                parts.push_back(part);
                part.clear();
            }
        }
        else
        {
            part += c;
        }
    }
    if (!part.empty())
        parts.push_back(part);

    if (is_absolute)
        parts.insert(parts.begin(), "/");
    return parts;
}

Node *CatalogTree::traverse_path(Node *start, const vector<string> &parts)
{
    Node *current = start;
    for (const auto &part : parts)
    {
        if (part == "..")
        {
            if (current->father)
                current = current->father;
        }
        else if (part == ".")
        {
            continue;
        }
        else if (part == "/")
        { // 处理绝对路径根目录
            current = root;
        }
        else
        {
            auto it = find_if(current->next.begin(), current->next.end(),
                              [&](Node *n)
                              { return n->name == part; });
            if (it == current->next.end())
                return nullptr;
            current = *it;
        }
    }
    return current;
}

void CatalogTree::find_recursive(Node *node, const string &target, const string &current_path, vector<string> &results, const string &type)
{
    if (!node)
        return;

    // 处理当前目录的文件匹配
    if (type.empty() || type == "file")
        for (const auto &file : node->file_list)
        {
            if (file == target)
            {
                string file_path;
                if (current_path.back() == '/')
                {
                    file_path = current_path + file;
                }
                else
                {
                    file_path = current_path + "/" + file;
                }
                results.push_back(file_path);
            }
        }

    // 处理当前目录名的匹配（除了根目录）
    if (node != root && node->name == target && (type.empty() || type == "dir"))
    {
        results.push_back(current_path + "/");
    }

    // 递归处理子目录
    for (Node *child : node->next)
    {
        string child_path;
        if (current_path == "/")
        {
            child_path = current_path + child->name;
        }
        else
        {
            child_path = current_path + "/" + child->name;
        }
        find_recursive(child, target, child_path, results, type);
    }
}

Node *CatalogTree::find_parent(const string &path, string &name)
{
    vector<string> parts = split_path(path);
    if (parts.empty())
        return nullptr;

    // 分离最后一级名称
    name = parts.back();
    parts.pop_back(); // 移除最后一级，保留父路径

    // 处理绝对路径和相对路径
    Node *start_node = (parts.empty() || parts[0] != "/") ? current : root;
    if (!parts.empty() && parts[0] == "/")
        parts.erase(parts.begin());

    // 遍历到父目录
    Node *parent = traverse_path(start_node, parts);
    return parent;
}

void CatalogTree::copy_file(Node *src_dir, const string &filename, Node *dest_dir)
{
    // 直接添加文件名到目标目录
    dest_dir->file_list.push_back(filename);
}

void CatalogTree::copy_directory(Node *src, Node *dest, bool force)
{
    // 复制文件列表
    for (const auto &file : src->file_list)
    {
        if (force)
        {
            auto it = find(dest->file_list.begin(), dest->file_list.end(), file);
            if (it != dest->file_list.end())
            {
                dest->file_list.erase(it);
            }
        }
        dest->file_list.push_back(file);
    }

    // 递归复制子目录
    for (Node *child : src->next)
    {
        Node *new_child = nullptr;
        auto existing_dir = find_if(dest->next.begin(), dest->next.end(),
                                    [&child](Node *n)
                                    { return n->name == child->name; });
        // 处理已存在目录
        if (existing_dir != dest->next.end())
        {
            if (!force)
                throw runtime_error("目录冲突: " + child->name);
            new_child = *existing_dir;
        }
        // 创建新目录
        else
        {
            new_child = new Node(child->name);
            new_child->father = dest;
            dest->next.push_back(new_child);
        }
        copy_directory(child, new_child, force);
    }
}

void CatalogTree::Dir()
{
    // 输出文件
    for (auto file : current->file_list)
        *output << file << '*' << endl;
    // 输出文件夹
    for (auto dir : current->next)
        *output << dir->name << endl;
}

void CatalogTree::Cd_1()
{
    // 如果为根节点，只返回一个/
    if (current == root)
    {
        *output << "/" << endl;
        return;
    }

    // 如果不是，从当前节点往上找
    string str = "";
    Node *ptr = current;
    while (ptr != root) // 不是根节点继续往上找
    {
        str = "/" + ptr->name + str;
        ptr = ptr->father;
    }

    *output << str << endl;
}

void CatalogTree::Cd_2()
{
    if (current == root) // 如果是根节点，没法返回父目录
        return;
    current = current->father;
}

void CatalogTree::Cd_3(const string &str)
{
    // 遍搜目录即可
    for (auto dir : current->next)
        if (dir->name == str)
        {
            current = dir;
            return;
        }
}

void CatalogTree::Mkdir(const string &str)
{
    // 确定这个目录是不是已经存在了
    for (auto dir : current->next)
        if (dir->name == str)
            return;
    // 没存在就创建
    Node *ptr = new Node(str, current);
    current->next.push_back(ptr); // 压入新节点
    // 思考：用set来存是不是更好，因为是红黑树
    sort(current->next.begin(), current->next.end(), cmp);
}

void CatalogTree::Mkfile(const string &str)
{
    // 确定这个文件是不是已经存在了
    for (auto file : current->file_list)
        if (file == str)
            return;
    // 没存在就创建
    current->file_list.push_back(str);
    sort(current->file_list.begin(), current->file_list.end()); // 字典序排序
}

void CatalogTree::Delete(const string &str)
{
    // 删除目录
    auto &dirs = current->next;
    dirs.erase(remove_if(dirs.begin(), dirs.end(),
                         [&](Node *node)
                         {
                             if (node->name == str)
                             {
                                 delete node;
                                 return true;
                             }
                             return false;
                         }),
               dirs.end());

    // 删除文件
    auto &files = current->file_list;
    files.erase(remove(files.begin(), files.end(), str), files.end());
}

void CatalogTree::Save(const string &str)
{ // 把指令树用建树指令表示出来
    ofstream ofs(str);
    if (!ofs)
    {
        *output << "无法保存文件" << endl;
        return;
    }
    save_tree(ofs, root, "");
}

void CatalogTree::Load(const string &str)
{ // DFS建树
    ifstream ifs(str);
    if (!ifs)
    {
        *output << "无法加载文件" << endl;
        return;
    }

    // 把旧树销毁
    function<void(Node *)> deleteTree = [&](Node *node) { // 函数内进行递归用lambda表达式很好
        for (Node *child : node->next)
            deleteTree(child);
        delete node;
    };
    deleteTree(root); // 递归删除树

    root = new Node("");
    current = root;
    vector<pair<size_t, Node *>> nodeStack; // 目录层级栈，模拟递归进行DFS
    /* 目录层级栈每个元素代表
        - first:  缩进级别（每4空格为一级）
        - second: 对应层级的目录节点指针
    */

    string line; // 读取文件每一行
    while (getline(ifs, line))
    {
        // 解析缩进级别
        size_t indent = 0; // 级别
        size_t pos = 0;    // 前置空格数
        // 每四个空格为一个级别
        while (pos < line.size() && line.substr(pos, 4) == "    ")
        {
            pos += 4;
            indent++;
        }

        // 去除缩进，提取有效内容
        string content = line.substr(pos);
        if (content.empty())
            continue;

        // 处理文件项
        // 修改文件处理逻辑
        if (content.back() == '*')
        {
            content.pop_back();
            Node *parent = root; // 默认父节点为根目录
            if (!nodeStack.empty())
            {
                parent = nodeStack.back().second;
            }
            parent->file_list.push_back(content);
            continue;
        }

        // 处理目录项
        // 调整目录栈到正确层级
        while (!nodeStack.empty() && nodeStack.back().first >= indent)
            nodeStack.pop_back();
        // 确定父目录
        Node *parent = nodeStack.empty() ? root : nodeStack.back().second;

        // 创建新目录节点
        Node *newNode = new Node(content, parent);
        parent->next.push_back(newNode);

        // 将新目录压入到栈种
        nodeStack.emplace_back(indent, newNode);
    }

    // 全树排序
    function<void(Node *)> sortTree = [&](Node *node)
    {
        // 排序当前目录的子目录
        sort(node->next.begin(), node->next.end(), cmp);
        // 排序当前目录的文件
        sort(node->file_list.begin(), node->file_list.end());
        // 递归处理子目录
        for (Node *child : node->next)
            sortTree(child);
    };
    sortTree(root); // 从根节点开始排序
}

void CatalogTree::Cd_4(const string &str)
{
    Node *original = current;
    vector<string> parts = split_path(str);

    // 判断路径类型
    bool is_absolute = !str.empty() && str[0] == '/';
    Node *start_node = is_absolute ? root : current;

    if (Node *target = traverse_path(start_node, parts))
    {
        current = target;
    }
    else
    {
        *output << "无效路径: " << str << endl;
        current = original; // 恢复原始位置
    }
}

void CatalogTree::Tree(int depth)
{
    print_tree(root, 0, depth, "", true); // 从根节点开始打印
}

void CatalogTree::Clear()
{
    system("cls");
}

void CatalogTree::Find(const string &name, const string &type)
{
    vector<string> results;
    find_recursive(root, name, "/", results, type);

    *output << "Found " << results.size() << " results:" << endl;
    for (const auto &path : results)
        *output << path << endl;
}

void CatalogTree::Cp(const string &src_path, const string &dest_path, bool force)
{
    // 解析源路径
    string src_name;
    Node *src_parent = find_parent(src_path, src_name);
    if (!src_parent)
        throw runtime_error("源路径不存在: " + src_path);

    // 确定源类型（文件/目录）
    bool is_file = false;
    Node *src_dir = nullptr;

    // 检查文件列表
    auto file_it = find(src_parent->file_list.begin(), src_parent->file_list.end(), src_name);
    if (file_it != src_parent->file_list.end())
    {
        is_file = true;
    }
    // 检查目录列表
    else
    {
        auto dir_it = find_if(src_parent->next.begin(), src_parent->next.end(),
                              [&src_name](Node *n)
                              { return n->name == src_name; });
        if (dir_it == src_parent->next.end())
        {
            throw runtime_error("源路径不存在: " + src_path);
        }
        src_dir = *dir_it;
    }

    // 解析目标路径
    string dest_name;
    Node *dest_parent = find_parent(dest_path, dest_name);

    // 处理目标路径为目录的情况（以/结尾）
    bool dest_is_directory = (!dest_path.empty() && dest_path.back() == '/');
    if (dest_is_directory && !dest_name.empty())
    {
        auto dir_it = find_if(dest_parent->next.begin(), dest_parent->next.end(),
                              [&dest_name](Node *n)
                              { return n->name == dest_name; });
        if (dir_it != dest_parent->next.end())
        {
            dest_parent = *dir_it; // 进入目标目录
            dest_name.clear();     // 使用源名称
        }
        else
        {
            throw runtime_error("目标目录不存在: " + dest_path);
        }
    }

    // 确定最终目标名称
    string final_dest_name = dest_name.empty() ? src_name : dest_name;

    // 非强制模式下的冲突检查
    if (!force)
    {
        bool conflict = false;
        // 检查文件冲突
        if (find(dest_parent->file_list.begin(), dest_parent->file_list.end(), final_dest_name) != dest_parent->file_list.end())
        {
            conflict = true;
        }
        // 检查目录冲突
        else if (find_if(dest_parent->next.begin(), dest_parent->next.end(),
                         [&final_dest_name](Node *n)
                         { return n->name == final_dest_name; }) != dest_parent->next.end())
        {
            conflict = true;
        }
        if (conflict)
        {
            throw runtime_error("目标已存在: " + dest_path + " (使用 -f 强制覆盖)");
        }
    }

    // 强制模式下的删除操作
    if (force)
    {
        // 删除同名文件
        auto file_it = find(dest_parent->file_list.begin(), dest_parent->file_list.end(), final_dest_name);
        if (file_it != dest_parent->file_list.end())
        {
            dest_parent->file_list.erase(file_it);
        }
        // 删除同名目录
        auto dir_it = find_if(dest_parent->next.begin(), dest_parent->next.end(),
                              [&final_dest_name](Node *n)
                              { return n->name == final_dest_name; });
        if (dir_it != dest_parent->next.end())
        {
            delete *dir_it; // 递归删除目录内容
            dest_parent->next.erase(dir_it);
        }
    }

    // 执行复制操作
    if (is_file)
    {
        // 文件复制（添加至目标文件列表）
        dest_parent->file_list.push_back(final_dest_name);
    }
    else
    {
        // 查找或创建目标目录
        Node *target_dir = nullptr;
        auto existing_dir = find_if(dest_parent->next.begin(), dest_parent->next.end(),
                                    [&final_dest_name](Node *n)
                                    { return n->name == final_dest_name; });
        // 目录已存在时直接使用
        if (existing_dir != dest_parent->next.end())
        {
            target_dir = *existing_dir;
        }
        // 创建新目录
        else
        {
            target_dir = new Node(final_dest_name);
            target_dir->father = dest_parent;
            dest_parent->next.push_back(target_dir);
        }
        // 递归复制目录内容
        copy_directory(src_dir, target_dir, force);
    }
}