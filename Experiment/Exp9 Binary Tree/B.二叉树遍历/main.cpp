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
private:
    binaryTreeNode<T> *root; // 根节点指针
    int treeSize;            // 树结点的数目
    binaryTreeNode<T> *treeRebuild(T *pre, int pre_l, int pre_r, T *in, int in_l, int in_r);

public:
    linkedBinaryTree() // 构造函数
    {
        root = NULL;
        treeSize = 0;
    }
    bool empty() const { return treeSize == 0; } // 判空函数
    int size() const { return treeSize; }
    binaryTreeNode<T> *get_root() { return root; }
    void preOrder(binaryTreeNode<T> *t);    // 前序遍历
    void inOrder(binaryTreeNode<T> *t);     // 中序遍历
    void postOrder(binaryTreeNode<T> *t);   // 后序遍历
    int height(binaryTreeNode<T> *t);       // 返回树的高度
    int num(binaryTreeNode<T> *t);          // 返回树的节点数
    void Rebuild(T *pre, T *in, int length) // 重构本树
    {
        root = treeRebuild(pre, 0, length - 1, in, 0, length - 1);
        treeSize = num(root);
    } // 树生成函数
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
    inOrder(t->leftChild);     // 遍历搜左孩子
    cout << t->element << ' '; // 输出根节点
    inOrder(t->rightChild);    // 遍历搜右孩子
}

template <class T>
void linkedBinaryTree<T>::postOrder(binaryTreeNode<T> *t)
{
    if (t == NULL)
        return;
    postOrder(t->leftChild);   // 遍历搜左孩子
    postOrder(t->rightChild);  // 遍历搜右孩子
    cout << t->element << ' '; // 输出根节点
}

template <class T>
int linkedBinaryTree<T>::height(binaryTreeNode<T> *t)
{
    if (t == NULL)
        return 0;
    return max(height(t->rightChild), height(t->leftChild)) + 1; // 递归求解树
}

template <class T>
int linkedBinaryTree<T>::num(binaryTreeNode<T> *t)
{
    int n = 0;
    if (t != NULL) // 递归求解
        n = num(t->leftChild) + num(t->rightChild) + 1;
    return n;
}

template <class T>
binaryTreeNode<T> *linkedBinaryTree<T>::treeRebuild(T *pre, int pre_l, int pre_r, T *in, int in_l, int in_r)
{
    if (pre_l > pre_r)
        return NULL;
    binaryTreeNode<T> *r = new binaryTreeNode<T>(pre[pre_l]);
    if (pre_l == pre_r)
        return r;
    int find = in_l;
    while (in[find] != pre[pre_l])
        find++;
    r->leftChild = treeRebuild(pre, pre_l + 1, pre_l + find - in_l, in, in_l, find - 1);
    r->rightChild = treeRebuild(pre, pre_l + find - in_l + 1, pre_r, in, find + 1, in_r);
    return r;
}

int main()
{
    int n;
    cin >> n;
    int *pre = new int[n];
    int *in = new int[n];
    for (int i = 0; i < n; i++)
        cin >> pre[i];
    for (int i = 0; i < n; i++)
        cin >> in[i];
    linkedBinaryTree<int> tree;
    tree.Rebuild(pre, in, n);
    tree.postOrder(tree.get_root());
    cout << endl;

    delete[] pre;
    delete[] in;

    return 0;
}
