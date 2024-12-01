# 数据结构与算法作业  2024/11/1

## 1.(P346.T6)

解：

1) 依次插入序列[4,12,8,16,6,18,24,2,14,3]于二叉搜索树的步骤如下：

<img src="扫描全能王 2024-11-15 23.09(1).jpg" alt="alt text" width="550px">

<div style="page-break-after: always;"></div>

2) 依次删除[6,14,16,4]的过程如下：

<img src="扫描全能王 2024-11-15 23.09_2(1).jpg" alt="alt text" width="550px">

<div style="page-break-after: always;"></div>

## 2.(P346.T10)

解：

根据我们的搜索树定义，我们在继承链表描述二叉树的基础上继承得到二叉搜索树的模板类，该类中新增两个函数：按搜索树规则插入结点，排序输出，前者就是基于与左右孩子数据比较大小实现的，后者只需要简单的中序遍历搜索树即可，完整类定义如下：

```c++
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
```

其中，`binarySearchTree`的`insert`函数如下所示：

```c++
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
```

利用下主函数，我们可以测试该代码正确性：

```c++
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
```

测试表明，我们可以正常的输出排序后结果。

下面，我们来分析该搜索树排序方法的时间复杂度：

- 最坏情况：树退化为链表

    插入一个元素需要沿着树的深度向下遍历，每次只访问一个子树。当树退化为链表时，树的高度为$O(n)$，因此插入一个元素的复杂度为$O(n)$
    如果有$n$个元素依次插入，最坏情况下总时间复杂度为$O(n^2)$

- 平均情况：树保持平衡
  
    对于一棵随机构造的二叉搜索树，树的平均高度为$O(\log n)$，因此单次插入复杂度为$O(\log n)$。插入$n$个元素的总复杂度为$O(n \log n)$

当然，中序遍历的复杂度为$O(n)$，小于插入过程，在渐进标记下可以不计。

总而言之，该算法的复杂度为：

- 最坏情况: 如果树退化为链表，整体复杂度为：$O(n^2)$

- 平均情况: 树保持平衡时，整体复杂度为：$O(n \log n)$

我们回忆一下插入排序和堆排序，可以发现他们的复杂度为：

插入排序：

- 最坏情况: $O(n^2)$

- 平均情况：$O(n^2)$

堆排序：

- 最坏情况: $O(n \log n)$

- 平均情况：$O(n \log n)$

我们可以发现，搜索树排序的平均复杂度与堆排是近似的，并且是强于插入排序的，但是在最坏情况下，该排序的时间复杂度是远大于堆排的，这正是因为我们的搜索树有可能会退化为链表，这一问题我们可以通过建立平衡搜索树（比如AVL树）解决。
