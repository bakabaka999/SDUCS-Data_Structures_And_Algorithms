#include <iostream>

using namespace std;

template <class T>
struct binaryTreeNode
{
    T element;
    binaryTreeNode<T> *leftChild;  // 左子树指针
    binaryTreeNode<T> *rightChild; // 右子树指针

    binaryTreeNode() { leftChild = rightChild = NULL; }
    binaryTreeNode(const T &theElement)
    {
        element = theElement;
        leftChild = rightChild = NULL;
    }
    binaryTreeNode(const T &theElement, binaryTreeNode<T> *theLeftChild, binaryTreeNode<T> *theRightNode)
    {
        element = theElement;
        leftChild = theLeftChild;
        rightChild = theRightNode;
    }
};

template <class T>
class linkedBinaryTree
{
protected:
    binaryTreeNode<T> *root; // 根节点指针
    int treeSize;            // 树结点的数目

public:
    linkedBinaryTree() // 构造函数
    {
        root = NULL;
        treeSize = 0;
    }
    bool empty() const { return treeSize == 0; } // 判空函数
    int size() const { return treeSize; }
    void preOrder(binaryTreeNode<T> *t); // 前序遍历
    void inOrder(binaryTreeNode<T> *t);  // 中序遍历
    void inOrder() { inOrder(root); }
    void postOrder(binaryTreeNode<T> *t); // 后序遍历
};

template <class T>
class binarySearchTree : public linkedBinaryTree<T>
{
public:
    void insert(const T &theElement);       // 按搜索树要求插入结点
    void sort_output() { this->inOrder(); } // 按顺序输出
};

template <class T>
void linkedBinaryTree<T>::preOrder(binaryTreeNode<T> *t)
{
    if (t == NULL)
        return;
    cout << t->element << ' '; // 输出根节点
    preOrder(t->leftChild);    // 遍历搜左孩子
    preOrder(t->rightChild);   // 遍历搜右孩子
}

template <class T>
void linkedBinaryTree<T>::inOrder(binaryTreeNode<T> *t)
{
    if (t == NULL)
        return;
    inOrder(t->leftChild);    // 遍历搜左孩子
    cout << t->element << ' '; // 输出根节点
    inOrder(t->rightChild);   // 遍历搜右孩子
}

template <class T>
void linkedBinaryTree<T>::postOrder(binaryTreeNode<T> *t)
{
    if (t == NULL)
        return;
    postOrder(t->leftChild);    // 遍历搜左孩子
    postOrder(t->rightChild);   // 遍历搜右孩子
    cout << t->element << ' '; // 输出根节点
}

template <class T>
void binarySearchTree<T>::insert(const T &theElement)
{ // 插入theElement.如存在相同数，直接覆盖
    binaryTreeNode<T> *p = this->root,
                      *pp = NULL;
    while (p != NULL)
    {
        pp = p;
        // p移到它的一个孩子结点
        if (theElement < p->element)
            p = p->leftChild; // 小结点移动到左孩子
        else if (theElement > p->element)
            p = p->rightChild; // 大节点移动到右孩子
        else                   // 说明存在该元素
            return;
    }
    // 为该元素创建一个新节点，然后与p连接
    binaryTreeNode<T> *newNode = new binaryTreeNode<T>(theElement);
    if (this->root != NULL)          // 树非空
        if (theElement < pp->element) // 往左插
            pp->leftChild = newNode;
        else
            pp->rightChild = newNode;
    else // 树为空
        this->root = newNode;
    this->treeSize++;
}

int main()
{
    binarySearchTree<int> search_tree;
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        int num;
        cin >> num;
        search_tree.insert(num);
    }
    search_tree.sort_output();
    cout << endl;
    return 0;
}
