#include <iostream>

using namespace std;

template <class T>
struct BinaryTreeNode // 二叉树结点
{
    T element;
    BinaryTreeNode<T> *leftChild;
    BinaryTreeNode<T> *rightChild;
    int leftSize; // 左子树元素个数，表示索引

    BinaryTreeNode(const T &theElement)
    {
        element = theElement;
        leftChild = NULL;
        rightChild = NULL;
        leftSize = 0;
    }
    BinaryTreeNode(const T &theElement, BinaryTreeNode<T> *theLeft, BinaryTreeNode<T> *theRight, int LeftSize)
    {
        element = theElement;
        leftChild = theLeft;
        rightChild = theRight;
        leftSize = LeftSize;
    }
};

template <class T>
class indexedBSTree
{
private:
    int size;
    BinaryTreeNode<T> *root; // 根节点
public:
    indexedBSTree()
    {
        root = NULL;
        size = 0;
    }
    int BST_Insert(const T &theElement); // 插入函数
    int BST_Find(const T &theElement);   // 查找函数
    int BST_Erase(const T &theElement);  // 删除函数
    int BST_Order_Search(int a);         // 按名次搜索函数
    int BSE_Order_Delete(int a);         // 按名次删除函数
};

template <class T>
int indexedBSTree<T>::BST_Insert(const T &theElement)
{
    BinaryTreeNode<T> *p = root;  // 当前操作节点指针
    BinaryTreeNode<T> *pp = NULL; // 当前操作节点父节点的指针
    int sum = 0;                  // 记录异或和
    while (p != NULL)
    {
        if (p->element < theElement) // 大值往右走
        {
            sum ^= p->element;
            pp = p;
            p = p->rightChild;
        }
        else if (p->element > theElement) // 小值往左走
        {
            sum ^= p->element;
            pp = p;
            p = p->leftChild;
        }
        else          // 恰好相等
            return 0; // 不需要插入
    }

    // 为新元素创造新结点，与pp相连
    BinaryTreeNode<T> *newNode = new BinaryTreeNode<T>(theElement);

    if (pp == NULL)
        root = newNode; // 空树，设为根节点
    else
    {
        if (theElement > pp->element) // 往右插
            pp->rightChild = newNode;
        else if (theElement < pp->element) // 往左插
            pp->leftChild = newNode;
    }

    size++;

    // 下面我们来计算各个元素的新索引
    p = root;
    while (p->element != theElement) // 遍搜左子树直到本元素
    {
        if (p->element < theElement)      // 插入元素在右边
            p = p->rightChild;            // 往右搜
        else if (p->element > theElement) // 插入元素在左边
        {
            p->leftSize++;
            p = p->leftChild;
        }
    }

    return sum;
}

template <class T>
int indexedBSTree<T>::BST_Find(const T &theElement)
{
    BinaryTreeNode<T> *p = root;
    int sum = 0;
    while (p != NULL && p->element != theElement)
    {
        sum ^= p->element;
        if (p->element > theElement)
            p = p->leftChild;
        else if (p->element < theElement)
            p = p->rightChild;
    }
    if (p == NULL) // 没找到
        return 0;

    sum ^= p->element;
    return sum;
}

template <class T>
int indexedBSTree<T>::BST_Erase(const T &theElement)
{
    BinaryTreeNode<T> *p = root;
    BinaryTreeNode<T> *pp = NULL;
    int sum = 0;
    while (p != NULL && p->element != theElement)
    {
        sum ^= p->element;
        pp = p;
        if (p->element > theElement)
            p = p->leftChild;
        else if (p->element < theElement)
            p = p->rightChild;
    }
    if (p == NULL) // 没找到
        return 0;
    sum ^= p->element;

    // 下面我们来计算各个元素的新索引
    p = root;
    while (p != NULL && p->element != theElement) // 遍搜左子树直到本元素
    {
        if (p->element < theElement)      // 插入元素在右边
            p = p->rightChild;            // 往右搜
        else if (p->element > theElement) // 插入元素在左边
        {
            p->leftSize--;
            p = p->leftChild;
        }
    }

    // 如果当前元素有两个孩子
    if (p->leftChild != NULL && p->rightChild != NULL)
    {
        // 我们找到右子树中的最小元素进行替换
        BinaryTreeNode<T> *min_right = p->rightChild;
        BinaryTreeNode<T> *delete_r = p;

        // 替换为右子树中的最小元素
        while (min_right->leftChild != NULL)
        {
            min_right->leftSize--;
            delete_r = min_right;             // 记录最小节点的父节点位置
            min_right = min_right->leftChild; // 向左移动
        }
        // 生成用于替换的结点
        BinaryTreeNode<T> *newNode = new BinaryTreeNode<T>(min_right->element, p->leftChild, p->rightChild, p->leftSize);

        // 开始替换
        if (pp == NULL) // 如果p为根节点
            root = newNode;
        else if (p == pp->leftChild) // 原本节点在父节点左边
            pp->leftChild = newNode;
        else // 原本节点在父节点右边
            pp->rightChild = newNode;

        if (delete_r == p) // 如果右子树根节点就是最小节点
            pp = newNode;
        else // 否则，delete_r更新为p的父节点
            pp = delete_r;

        delete p;
        p = min_right; // 移动到刚刚用于替换的结点进行下一步操作
        // 这里，这个节点还没有被删除，但是这个结点一定是没有孩子或者只有一个孩子的，可以按照下面的通用逻辑处理
    }

    // 如果当前元素只有一个孩子
    BinaryTreeNode<T> *c;
    if (p->leftChild != NULL)
        c = p->leftChild;
    else
        c = p->rightChild;

    // 如果删除节点位于根节点
    if (p == root)
        root = c;
    // 删除节点不是根节点
    else
    {
        if (p == pp->leftChild) // p是pp的左孩子
            pp->leftChild = c;
        else
            pp->rightChild = c;
    }
    size--;
    delete p;
    return sum;
}

template <class T>
int indexedBSTree<T>::BST_Order_Search(int a)
{
    BinaryTreeNode<T> *p = root;
    int sum = 0;
    while (p != NULL && p->leftSize != a)
    {
        sum ^= p->element;
        if (p->leftSize > a) // 节点索引更大，证明在左孩子中
            p = p->leftChild;
        else // 在右孩子中
        {
            a = a - p->leftSize - 1;
            p = p->rightChild;
        }
    }
    // 没找到
    if (p == NULL)
        return 0;
    // 找到了
    sum ^= p->element;
    return sum;
}

template <class T>
int indexedBSTree<T>::BSE_Order_Delete(int a)
{
    BinaryTreeNode<T> *p = root;
    int sum = 0;
    while (p != NULL && p->leftSize != a)
    {
        sum ^= p->element;
        if (p->leftSize > a) // 节点索引更大，证明在左孩子中
            p = p->leftChild;
        else if (p->leftSize < a) // 在右孩子中
        {
            a = a - p->leftSize - 1;
            p = p->rightChild;
        }
    }
    // 没找到
    if (p == NULL)
        return 0;
    sum ^= p->element;
    int theElement = p->element;
    this->BST_Erase(theElement);
    return sum;
}

int main()
{
    int m;
    cin >> m;
    indexedBSTree<int> bsTree;
    for (int i = 0; i < m; i++)
    {
        int a, b;
        cin >> a >> b;
        switch (a)
        {
        case 0:
        {
            cout << bsTree.BST_Insert(b) << endl;
            break;
        }
        case 1:
        {
            cout << bsTree.BST_Find(b) << endl;
            break;
        }
        case 2:
        {
            cout << bsTree.BST_Erase(b) << endl;
            break;
        }
        case 3:
        {
            b--;
            cout << bsTree.BST_Order_Search(b) << endl;
            break;
        }
        case 4:
        {
            b--;
            cout << bsTree.BSE_Order_Delete(b) << endl;
            break;
        }
        }
    }
    return 0;
}
