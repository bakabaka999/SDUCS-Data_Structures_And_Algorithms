#include <iostream>

using namespace std;

template <class T>
class minHeap
{
private:
    T *heap;
    int arrayLength;
    int heapSize;

public:
    minHeap(int initialCapacity = 10); // 构造函数
    ~minHeap() { delete[] heap; }      // 析构函数

    void initialize(T *theHeap, int n); // 堆初始化
    void push(const T &theElement);     // 堆插入
    const T &top()
    {
        if (heapSize == 0)
            throw runtime_error("Heap is empty!");
        return heap[1];
    } // 堆顶元素读取
    T pop();                    // 堆顶元素弹出
    void heapSort(T *a, int n); // 堆排序
};

template <class T>
minHeap<T>::minHeap(int initialCapacity)
{
    arrayLength = initialCapacity + 1;
    heapSize = 0;
    heap = new T[arrayLength];
}

template <class T>
void minHeap<T>::initialize(T *theHeap, int n)
{
    // 传入新数组（不一定完成堆化）
    delete[] heap;
    heap = new T[n + 1];
    for (int i = 1; i <= n; i++)
        heap[i] = theHeap[i];
    heap[0] = T();
    arrayLength = n + 1;
    heapSize = n;

    // 堆化(从最后一个有孩子的结点开始，调整子树为小根堆)
    for (int root = heapSize / 2; root >= 1; root--)
    {
        T rootElement = heap[root]; // 找到父节点元素
        
        // 为父节点rootElement寻找位置
        int child = 2 * root;
        while (child <= heapSize) // 孩子不能超过堆大小
        {
            // heap[child]取兄弟中较小者
            if (child < heapSize && heap[child] > heap[child + 1])
                child++;
            // 是否可以将rootElement放到heap[root]上
            if (rootElement <= heap[child])
                break;                     // 可以
            heap[child / 2] = heap[child]; // 不可以
            child *= 2;                    // 下移孩子完成下方子树
        }
        heap[child / 2] = rootElement;
    }
}

template <class T>
void minHeap<T>::push(const T &theElement)
{ // 把theElement插到合适的地方
    // 空间不足时扩容
    if (heapSize == arrayLength - 1) // 堆满了，加倍空间
    {
        arrayLength *= 2;
        T *newHeap = new T[arrayLength];
        for (int i = 1; i <= heapSize; i++)
            newHeap[i] = heap[i];
        delete[] heap;
        heap = newHeap;
    }

    // 寻找插入位置
    // currentNode从新叶子向上移动
    int currentNode = ++heapSize;
    while (currentNode != 1 && heap[currentNode / 2] > theElement) // 该节点不是根节点且父节点元素大于插入元素
    {
        // 不能把TheElement插到这
        heap[currentNode] = heap[currentNode / 2]; // 元素向下移动
        currentNode /= 2;                          // currentNode移向双亲
    }

    heap[currentNode] = theElement;
}

template <class T>
T minHeap<T>::pop()
{
    // 如果堆为空，直接返回
    if (heapSize == 0)
        throw runtime_error("Heap is empty!");
    // 删除最后一个元素，然后重新建堆
    T minElement = heap[1]; // 保存堆顶元素以便返回
    T lastElement = heap[heapSize--];

    // 从根开始，为最后一个元素寻找位置
    int currentNode = 1,
        child = 2;            // currentNode的孩子
    while (child <= heapSize) // 建堆过程
    {
        // heap[child]是currentNode的更小的孩子
        if (child < heapSize && heap[child] > heap[child + 1])
            child++;

        // 可以把lastElement放在heap[currentNode]么
        if (lastElement <= heap[child])
            break; // 可以
        // 不可以
        heap[currentNode] = heap[child];
        currentNode = child;
        child *= 2;
    }
    heap[currentNode] = lastElement;

    return minElement;
}

template <class T>
void minHeap<T>::heapSort(T *a, int n)
{
    T *tempHeap = new T[n + 1];
    for (int i = 1; i <= n; i++)
        tempHeap[i] = a[i];
    tempHeap[0] = T();

    initialize(tempHeap, n);

    for (int i = n; i >= 1; i--)
    {
        cout << heap[1] << " ";
        pop();
    }
    cout << endl;

    delete[] tempHeap;
}

int main()
{
    int n;
    cin >> n;
    int *arr = new int[n + 1];
    for (int i = 1; i <= n; i++)
        cin >> arr[i];

    minHeap<int> min_heap;
    min_heap.initialize(arr, n);
    cout << min_heap.top() << endl;

    int m;
    cin >> m;
    for (int i = 0; i < m; i++)
    {
        int oper;
        cin >> oper;
        switch (oper)
        {
        case 1:
        {
            int num;
            cin >> num;
            min_heap.push(num);
            cout << min_heap.top() << endl;
            break;
        }
        case 2:
        {
            try
            {
                min_heap.pop();
                cout << min_heap.top() << endl;
            }
            catch (const runtime_error &e)
            {
                cout << e.what() << endl;
            }
            break;
        }
        case 3:
        {
            int nn;
            cin >> nn;
            int *arr2 = new int[nn + 1];
            for (int j = 1; j <= nn; j++)
                cin >> arr2[j];
            min_heap.heapSort(arr2, nn);
            delete[] arr2;
            break;
        }
        }
    }
    delete[] arr;

    return 0;
}