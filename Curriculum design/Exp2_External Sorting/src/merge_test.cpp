#include "ExternalSorting.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

// 生成随机二进制测试文件
void generate_test_file(const string &filename, size_t num_elements)
{
    ofstream ofs(filename, ios::binary);
    srand(time(0));

    for (size_t i = 0; i < num_elements; ++i)
    {
        int val = rand() % 1000000; // 生成0-999999的随机数
        ofs.write(reinterpret_cast<const char *>(&val), sizeof(int));
    }
    ofs.close();
    cout << "生成测试文件 " << filename << "，包含 " << num_elements << " 个整数" << endl;
}

// 验证文件是否有序
bool verify_sorted_file(const string &filename)
{
    ifstream ifs(filename, ios::binary);
    if (!ifs)
    {
        cerr << "无法打开验证文件" << endl;
        return false;
    }

    int prev, current;
    streamsize count = 0;

    // 读取第一个元素
    ifs.read(reinterpret_cast<char *>(&prev), sizeof(int));
    if (!ifs)
        return true; // 空文件视为有序

    // 逐个验证后续元素
    while (ifs.read(reinterpret_cast<char *>(&current), sizeof(int)))
    {
        count++;
        if (current < prev)
        {
            cout << "排序错误发现：" << endl;
            cout << "位置 " << count << ": " << prev << " > " << current << endl;
            ifs.close();
            return false;
        }
        prev = current;
    }

    ifs.close();
    cout << "验证通过：文件共 " << count + 1 << " 个元素有序" << endl;
    return true;
}

// 验证输入输出文件元素数量一致
bool verify_element_count(const string &input_file, const string &output_file)
{
    ifstream ifs_in(input_file, ios::binary | ios::ate);
    ifstream ifs_out(output_file, ios::binary | ios::ate);

    streamsize in_size = ifs_in.tellg();
    streamsize out_size = ifs_out.tellg();

    ifs_in.close();
    ifs_out.close();

    if (in_size != out_size)
    {
        cerr << "文件大小不一致！输入: " << in_size
             << " 字节, 输出: " << out_size << " 字节" << endl;
        return false;
    }

    cout << "元素数量验证通过" << endl;
    return true;
}

int main()
{
    const string input_file = "test_data.bin";
    const string output_file = "sorted_data.bin";
    const size_t data_size = 100000; // 100000个整数 ≈ 400KB

    // 生成测试数据
    generate_test_file(input_file, data_size);

    try
    {
        // 初始化外排序器（4路归并）
        ExternalSorter<int> sorter(4, input_file, output_file);

        // 执行排序
        cout << "开始外排序..." << endl;
        sorter.sort();
        cout << "排序完成" << endl;

        // 新增统计输出
        cout << "\n==== 磁盘访问统计 ====" << endl;
        cout << "读操作次数: " << sorter.getReadCount() << endl;
        cout << "写操作次数: " << sorter.getWriteCount() << endl;
        cout << "总访问次数: " << sorter.getReadCount() + sorter.getWriteCount() << endl;

        // 验证结果
        if (!verify_element_count(input_file, output_file))
            return 1;
        if (!verify_sorted_file(output_file))
            return 1;

        cout << "所有测试通过！" << endl;
    }
    catch (const exception &e)
    {
        cerr << "发生异常: " << e.what() << endl;
        return 1;
    }

    // 清理测试文件（可选）
    // remove(input_file.c_str());
    // remove(output_file.c_str());

    return 0;
}