#include <bits/stdc++.h>

using namespace std;

template <class T>
class arrayList
{
private:
    T *element;      // 储存元素的一维数组
    int arrayLength; // 数组容量
    int listSize;    // 元素个数
public:
    arrayList(int initialCapacity = 10); // 普通构造函数
    arrayList(const arrayList<T> &arr);  // 复制构造函数
    ~arrayList() { delete[] element; }

    int get_Size() { return listSize; }

    void checkIndex(int theIndex) const;
    void push_back(const T &theElement); // 尾插入函数
    void erase(int theIndex);            // 删除函数
    T &get(int theIndex);                // 获取元素
};

class Contact
{
private:
    string Name;
    string Telephone;
    int Class;
    int Dorm; // 宿舍
public:
    Contact() {}
    void Input(); // 传入值

    // 对外接口
    string get_Name() { return Name; }
    string get_Telephone() { return Telephone; }
    int get_Class() { return Class; }
    int get_Dorm() { return Dorm; }

    void change_Tele(string &tele) { Telephone = tele; }
    void change_Class(int cla) { Class = cla; }
    void change_Dorm(int d) { Dorm = d; }
};

class Contact_List
{
private:
    arrayList<Contact *> contact_list;

public:
    Contact_List() {}
    ~Contact_List() {}

    void Insert_Contact();                   // 插入联系人
    void Delete_Contact(const string &name); // 删除联系人
    void Edit_Contact(const string &name);   // 修改联系人
    void Search_Contact(const string &name); // 查找联系人
    void Output_Contact(int cla);            // 输出异或值
};

template <class T>
arrayList<T>::arrayList(int initialCapacity)
{ // 构造函数
    arrayLength = initialCapacity;
    element = new T[arrayLength];
    listSize = 0;
}

template <typename T>
arrayList<T>::arrayList(const arrayList<T> &theList)
{ // 复制构造函数
    arrayLength = theList.arrayLength;
    listSize = theList.listSize;
    element = new T[arrayLength];
    std::copy(theList.element, theList.element + listSize, element);
}

template <typename T>
void arrayList<T>::checkIndex(int theIndex) const
{
    if (theIndex < 0 || theIndex >= listSize)
    {
        std::ostringstream s;
        s << "index = " << theIndex << "size = " << listSize;
        std::cout << s.str() << std::endl;
        exit(0);
    }
}

template <typename T>
void arrayList<T>::push_back(const T &theElement)
{
    if (listSize == arrayLength)
    {                     // 如果线性表满了
        arrayLength *= 2; // 二倍扩大线性表大小
        T *newElement = new T[arrayLength];
        for (int i = 0; i < listSize; ++i)
        {
            newElement[i] = element[i];
        }
        delete[] element; // 释放旧数组
        element = newElement;
    }
    element[listSize++] = theElement; // 尾插入
}

template <typename T>
void arrayList<T>::erase(int theIndex)
{
    checkIndex(theIndex);
    std::copy(element + theIndex + 1, element + listSize, element + theIndex);
    element[--listSize].~T();
}

template <typename T>
T &arrayList<T>::get(int theIndex)
{ // 若存在返回theIndex的元素
    checkIndex(theIndex);
    return element[theIndex];
}

void Contact::Input()
{
    cin >> Name >> Telephone >> Class >> Dorm;
}

void Contact_List::Insert_Contact()
{
    Contact *c = new Contact;
    c->Input();
    contact_list.push_back(c);
}

void Contact_List::Delete_Contact(const string &name)
{
    for (int i = 0; i < contact_list.get_Size(); i++) // 遍历所有元素
    {
        if (contact_list.get(i)->get_Name() == name)
        {
            Contact *p = contact_list.get(i);
            delete p;              // 先删内容
            contact_list.erase(i); // 在消除指针元素
        }
    }
}

void Contact_List::Edit_Contact(const string &name)
{
    int index;
    for (int i = 0; i < contact_list.get_Size(); i++) // 遍历所有元素
        if (contact_list.get(i)->get_Name() == name)
            index = i;
    int oper;
    cin >> oper;
    switch (oper)
    {
    case 1:
    {
        string tele;
        cin >> tele;
        contact_list.get(index)->change_Tele(tele);
    }
    break;
    case 2:
    {
        int cla;
        cin >> cla;
        contact_list.get(index)->change_Class(cla);
    }
    break;
    case 3:
    {
        int d;
        cin >> d;
        contact_list.get(index)->change_Dorm(d);
    }
    break;
    }
}

void Contact_List::Search_Contact(const string &name)
{
    for (int i = 0; i < contact_list.get_Size(); i++) // 遍历所有元素
        if (contact_list.get(i)->get_Name() == name)
        {
            cout << "1" << endl;
            return;
        }
    cout << "0" << endl;
}

void Contact_List::Output_Contact(int cla)
{
    int sum = 0;
    for (int i = 0; i < contact_list.get_Size(); i++) // 遍历所有元素
        if (contact_list.get(i)->get_Class() == cla)
            sum ^= contact_list.get(i)->get_Dorm();
    cout << sum << endl;
}

int main()
{
    Contact_List con_list;
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        int oper_1;
        cin >> oper_1;
        switch (oper_1)
        {
        case 0:
            con_list.Insert_Contact();
            break;
        case 1:
        {
            string name_1;
            cin >> name_1;
            con_list.Delete_Contact(name_1);
        }
        break;
        case 2:
        {
            string name_2;
            cin >> name_2;
            con_list.Edit_Contact(name_2);
        }
        break;
        case 3:
        {
            string name_3;
            cin >> name_3;
            con_list.Search_Contact(name_3);
        }
        break;
        case 4:
        {
            int cl;
            cin >> cl;
            con_list.Output_Contact(cl);
        }
        break;
        }
    }
    return 0;
}