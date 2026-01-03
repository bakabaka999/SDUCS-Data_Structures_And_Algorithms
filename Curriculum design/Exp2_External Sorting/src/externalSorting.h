#ifndef EXTERNAL_SORTING_H
#define EXTERNAL_SORTING_H

#include "loserTree.h"
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cassert>
#include <string>
#include <memory>
#include <ctime>

#define TEMP_INITIAL_RUN_PREFIX "temp_initial_run_"
#define TEMP_MERGED_RUN_PREFIX "temp_merged_run_"
#define BUFFER_SIZE 4096

using namespace std;

template <class T>
class ExternalSorter
{ // 外排序器
private:
    int m_k;                       // 归并路数
    string input_file;             // 输入文件名
    string output_file;            // 输出文件名
    vector<string> m_initial_runs; // 初始归并段
    vector<string> m_runs;         // 当前归并段列表
    vector<T> output_buffer;       // 输出缓冲区

    int read_count = 0;  // 磁盘读操作计数器
    int write_count = 0; // 磁盘写操作计数器

    // 输入流封装结构体
    // 功能：管理单个归并段的读取，实现块预读机制
    struct InputStream
    {
        ifstream file;      // 文件输入流
        vector<T> buffer;   // 块缓冲区
        size_t pos = 0;     // 当前读取位置
        bool active = true; // 流是否还有数据
        int &read_cnt_ref;  // 引用磁盘读次数

        // 构造函数：打开文件并加载首块数据
        InputStream(const string &filename, int &read_cnt) : buffer(BUFFER_SIZE / sizeof(T)), read_cnt_ref(read_cnt)
        {
            file.open(filename, ios::binary);
            refill(); // 初始化时立即填充首块
        }

        // 块填充方法
        void refill()
        {
            // 从文件读取完整块（BUFFER_SIZE字节）
            file.read(reinterpret_cast<char *>(buffer.data()), BUFFER_SIZE);
            read_cnt_ref++; // 统计每次块读取
            // 计算实际读取的元素数量
            streamsize count = file.gcount() / sizeof(T);
            buffer.resize(count);     // 调整缓冲区大小
            pos = 0;                  // 重置读取位置
            active = !buffer.empty(); // 无数据时标记为失效
            if (!active)
                file.close(); // 自动关闭失效流
        }

        // 获取下一个元素
        T next()
        {
            if (pos >= buffer.size())
            { // 当前块已读完
                if (file)
                    refill(); // 尝试读取下一块
                else
                    active = false; // 标记流失效
            }
            return active ? buffer[pos++]             // 返回当前元素
                          : numeric_limits<T>::max(); // 失效流返回最大值（败者树约定）
        }
    };

    // 初始归并段生成器
    class RunGenerator
    {
    private:
        vector<T> buffer_pool;  // 内存缓冲区
        ExternalSorter &sorter; // 外部排序器引用，用于统计
        int run_count = 0;      // 生成归并段编号

    public:
        RunGenerator(ExternalSorter &sorter_ref) : sorter(sorter_ref)
        {
            buffer_pool.reserve(BUFFER_SIZE / sizeof(T)); // 容量=块可容纳元素数
        }

        void add_data(const T &data)
        { // 把data添加到归并段里
            buffer_pool.push_back(data);
            if (buffer_pool.size() >= BUFFER_SIZE / sizeof(T))
                flush_buffer(); // 归并段满了就flush buffer传出归并段元素
        }

        // 最终刷新缓冲区
        void final_flush()
        {
            if (!buffer_pool.empty())
                flush_buffer();
        }

        int get_run_count() const { return run_count; }

        // 排序内存数据并写入临时文件
        void flush_buffer()
        {
            // 步骤1：内排序
            std::sort(buffer_pool.begin(), buffer_pool.end());

            // 步骤2：生成临时文件名
            string fname = TEMP_INITIAL_RUN_PREFIX + to_string(run_count++);

            // 步骤3：写入磁盘（块写入）
            ofstream ofs(fname, ios::binary);
            ofs.write(reinterpret_cast<const char *>(buffer_pool.data()),
                      buffer_pool.size() * sizeof(T));

            // 步骤4：更新读写操作统计
            sorter.read_count++;  // 块读取在这里算了
            sorter.write_count++; // 无论是否写满块，均计为一次写操作

            buffer_pool.clear(); // 清空缓冲区
        }
    };

public:
    // 构造函数：初始化参数
    ExternalSorter(int k, const string &input, const string &output) : m_k(k), input_file(input), output_file(output) {}
    void generate_initial_runs();                             // 生成初始归并段
    void kway_merge();                                        // k路归并
    void block_copy(const string &src, vector<string> &dest); // 实现块复制
    void block_copy(const string &src, const string &dest);   // 实现最终结果复制
    void sort()
    { // 完成外排序
        generate_initial_runs();
        kway_merge();
    }

    // 获取统计信息
    int getReadCount() const { return read_count; }
    int getWriteCount() const { return write_count; }
};

template <class T>
void ExternalSorter<T>::generate_initial_runs()
{
    ifstream ifs(input_file, ios::binary); // 先读取文件
    assert(ifs && "Input file open failed");

    RunGenerator run_generator(*this);
    T data;
    // 这里循环里面的是：把二进制数据读取成字符串传入data在转换
    while (ifs.read(reinterpret_cast<char *>(&data), sizeof(T)))
        run_generator.add_data(data); // 填充缓冲区
    run_generator.final_flush();      // 处理最后未满的缓冲区

    ifs.close();

    // 记录所有的初始归并段文件名
    for (int i = 0; i < run_generator.get_run_count(); i++)
        m_initial_runs.push_back(TEMP_INITIAL_RUN_PREFIX + to_string(i));

    m_runs = m_initial_runs; // 初始化当前归并段列表
}

template <class T>
void ExternalSorter<T>::kway_merge()
{
    if (m_runs.empty())
    { // 新增：处理无归并段的情况
        ofstream ofs(output_file, ios::binary);
        ofs.close(); // 创建空文件
        return;
    }

    vector<string> current_runs = m_runs;           // 当前需要处理的归并段列表
    int merge_round = 0;                            // merge轮回数
    output_buffer.reserve(BUFFER_SIZE / sizeof(T)); // 预分配输出缓冲区

    // 多轮归并，直到只剩一个归并段
    while (current_runs.size() > 1)
    {
        vector<string> new_runs; // 存储本轮生成的新归并段

        // 分批次处理，每轮处理 m_k 个归并段
        for (int start = 0; start < current_runs.size(); start += m_k)
        {
            int end = min(start + m_k, (int)current_runs.size());
            int actual_k = end - start; // 实际合并的路数（可能小于m_k）

            // 处理单路归并（直接复制文件）
            if (actual_k == 1)
            {
                block_copy(current_runs[start], new_runs);
                continue;
            }

            // 正式的多路归并
            // 步骤1：初始化k个输入流
            vector<unique_ptr<InputStream>> inputs; // 空容器
            inputs.reserve(actual_k);               // 预分配内存
            vector<T> current_values(actual_k, numeric_limits<T>::max());

            // 初始化每个输入流
            for (int i = 0; i < actual_k; ++i)
            {
                inputs.push_back(make_unique<InputStream>(current_runs[start + i], this->read_count)); // 正确添加元素
                current_values[i] = inputs.back()->next();
            }

            // 步骤2：构建输者树
            LoserTree<T> tree(actual_k);
            for (int i = 0; i < actual_k; ++i)
            {
                tree.rePlay(i + 1, current_values[i]); // 初始化各选手
            }

            // 步骤3：准备输出文件
            string new_run = TEMP_MERGED_RUN_PREFIX + to_string(merge_round++) + "_" + to_string(new_runs.size());
            ofstream ofs(new_run, ios::binary);
            output_buffer.clear(); // 清空成员缓冲区

            // 步骤4：归并循环
            while (tree.getWinner() < numeric_limits<T>::max())
            {
                // 获取当前最小值
                int winner_idx = tree.getWinnerIndex() - 1; // 胜者索引
                output_buffer.push_back(tree.getWinner());  // 写入缓冲区

                // 缓冲区满时写入磁盘
                if (output_buffer.size() >= BUFFER_SIZE / sizeof(T))
                {
                    ofs.write(reinterpret_cast<char *>(output_buffer.data()),
                              output_buffer.size() * sizeof(T));
                    write_count++;         // 块写入统计
                    output_buffer.clear(); // 清空缓冲区
                }

                // 更新胜者对应的输入流
                auto &stream = *inputs[winner_idx];
                current_values[winner_idx] = stream.next();
                // 更新败者树
                tree.rePlay(winner_idx + 1, current_values[winner_idx]);
            }

            // 步骤5：写入剩余数据
            if (!output_buffer.empty())
            {
                ofs.write(reinterpret_cast<char *>(output_buffer.data()),
                          output_buffer.size() * sizeof(T));
                write_count++; // 最终块写入统计
            }
            new_runs.push_back(new_run); // 记录新生成的归并段
        }
        // 清理上一轮临时文件（保留初始归并段）
        if (current_runs != m_runs)
            for (auto &f : current_runs)
                remove(f.c_str());
        current_runs = new_runs; // 更新当前归并段集合
    }

    // 最终结果处理
    if (!current_runs.empty())
        block_copy(current_runs[0], output_file); // 复制到最终输出文件

    // 清理初始归并段
    for (auto &f : m_runs)
        remove(f.c_str());
}

template <class T>
void ExternalSorter<T>::block_copy(const string &src, vector<string> &dest)
{
    string new_run = TEMP_MERGED_RUN_PREFIX + to_string(time(nullptr)) // 时间戳
                     + "_" + to_string(dest.size());                   // 添加唯一性标识
    ifstream ifs(src, ios::binary);
    ofstream ofs(new_run, ios::binary);
    T buffer[BUFFER_SIZE / sizeof(T)]; // 局部块缓冲区

    // 块复制循环
    while (ifs)
    {
        // 读取块数据
        ifs.read(reinterpret_cast<char *>(buffer), BUFFER_SIZE);
        streamsize bytes = ifs.gcount(); // 实际读取字节数
        if (bytes > 0)
            ofs.write(reinterpret_cast<char *>(buffer), bytes);
    }
    dest.push_back(new_run); // 记录新生成的文件
}

template <class T>
void ExternalSorter<T>::block_copy(const string &src, const string &dest)
{
    ifstream ifs(src, ios::binary);
    ofstream ofs(dest, ios::binary);
    T buffer[BUFFER_SIZE / sizeof(T)];

    // 简单块复制
    while (ifs)
    {
        ifs.read(reinterpret_cast<char *>(buffer), BUFFER_SIZE);
        streamsize bytes = ifs.gcount();
        if (bytes > 0)
        {
            ofs.write(reinterpret_cast<char *>(buffer), bytes);
        }
    }
}

#endif
