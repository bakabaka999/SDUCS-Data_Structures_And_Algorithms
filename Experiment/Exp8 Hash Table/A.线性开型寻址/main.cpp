#include <iostream>

using namespace std;

template <class E, class K>
class HashTable
{
private:
    int hSearch(const K &k) const; // 按照散列函数查找合适位置（为空或有相同值）
    int D;                         // 散列函数的除数
    E *hash_table;                 // 散列数组
    bool *empty;                   // 散列数组各个桶空满标志
public:
    HashTable(int divisor = 11); // 构造函数
    ~HashTable()                 // 析构函数
    {
        delete[] hash_table;
        delete[] empty;
    }
    void Search(const K &k, E &e);       // 查找功能
    HashTable<E, K> &Insert(const E &e); // 插入功能
    int Delete(const E &e);              // 删除功能
};

template <class E, class K>
HashTable<E, K>::HashTable(int divisor)
{
    D = divisor;
    hash_table = new E[D]; // 哈希表大小与除数相同
    empty = new bool[D];
    for (int i = 0; i < D; i++)
        empty[i] = true;
}

template <class E, class K>
int HashTable<E, K>::hSearch(const K &k) const
{
    int index = k % D;      // 由取余得到哈希后的位置
    int real_index = index; // 实际位置
    do
    {
        if (empty[real_index] || hash_table[real_index] == k) // 当找到合适位置，返回该位置
            return real_index;
        real_index = (real_index + 1) % D; // 前移索引
    } while (real_index != index); // 没转一圈就循环
    return real_index;
}

template <class E, class K>
void HashTable<E, K>::Search(const K &k, E &e)
{
    int place = hSearch(k);
    if (empty[place] || hash_table[place] != k) // 当查找到的位置为空或不是对应值（说明没存）
        cout << -1 << endl;
    else
        cout << place << endl;
}

template <class E, class K>
HashTable<E, K> &HashTable<E, K>::Insert(const E &e)
{
    K k = e; // 在这里，我们关键字与值相等
    int place = hSearch(k);
    if (empty[place]) // 为空说明没有且可插
    {
        empty[place] = false;
        hash_table[place] = e;
        cout << place << endl;
    }
    else // 不是空说明存在
        cout << "Existed" << endl;
    return *this;
}

template <class E, class K>
int HashTable<E, K>::Delete(const E &e)
{
    K k = e;
    int place = hSearch(k);
    int Count = 0;
    if (empty[place]) // 说明不存在这个元素
    {
        cout << "Not Found" << endl;
        return -1;
    }
    else if (hash_table[place] == k) // 说明存在，应该删
    {
        empty[place] = true; // 先判定其为空
        // 创建两个索引用于移动元素
        int index_1 = place; // 指向待移动元素
        int index_2 = place; // 指向空位所在地
        // 用于储存某元素哈希值
        int hash_index;

        do
        {
            index_1 = (index_1 + 1) % D;          // 向前移动索引1
            if (empty[index_1])                   // 如果为空
                break;                            // 直接退出循环
            hash_index = hash_table[index_1] % D; // 求解该元素哈希值

            if ((index_1 != hash_index && index_1 > index_2 && hash_index <= index_2) || (index_1 < hash_index && (index_2 < index_1 || index_2 >= hash_index)))
            { // 对应 hash_index--index_2--index_1的情况
                empty[index_2] = false;
                hash_table[index_2] = hash_table[index_1];
                empty[index_1] = true;
                Count++;
                index_2 = index_1; // 移动索引
            }
        } while (!empty[index_1 + 1] && (index_1 + 1) != index_2);
    }
    cout << Count << endl;
    return Count;
}

int main()
{
    int D, m;
    cin >> D >> m;
    HashTable<int, int> table(D);
    for (int i = 0; i < m; i++)
    {
        int opt, x;
        cin >> opt >> x;
        switch (opt)
        {
        case 0:
        {
            table.Insert(x);
            break;
        }
        case 1:
        {
            table.Search(x, x);
            break;
        }
        case 2:
        {
            table.Delete(x);
            break;
        }
        }
    }
    return 0;
}