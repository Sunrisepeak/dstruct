# DStruct

DStruct 是一个易于移植且结构简洁的数据结构模板库

[**English**](README.en.md)

> 当前状态-基础功能开发中

## 特性/目标

- 易于移植，不依赖标准库(std)和其它三方库（只需要实现 dstruct-port.h）
- 易于使用（数据结构和算法分离设计，统一接口，std标准库风格）
- 易于学习/定制和改进（结构简洁且只有头文件）
- 现代 C++ / 泛型结构，包含多样丰富的数据结构


---

## 使用DStruct

> **默认dstruct-port.h 是由libc实现的, 如果是嵌入式设备或其他情况, 可自行实现dstruct-port.h**

### 1. 下载源码

> 下载源码到(你的项目或其他选定的)本地目录

```
git clone git@github.com:Sunrisepeak/DStruct.git
```

### 2. 引用项目的dstruct.hpp接口文件

> 添加DStruct项目的根目录到include索引目录 `-IyourPath/DStruct`

```cpp
#include <iostream>

#include "dstruct.hpp"

int main() {

    std::cout << "\n\nTesting: " << __FILE__ << std::endl;

    dstruct::Array<int, 10> arr(2); // 0. test cntor

    decltype(arr)::ValueType val = 6;

    arr[0] = arr[-1] = val;         // 1. test subscript access & assignment

    for (int i = 0; i < arr.size(); i++) { // 3.  positive / negative subscript access
        std::cout << arr[-(i + 1)] << " : " << arr[i] << std::endl;
    }

    return 0;
}
```





## 数据结构列表

| 类别  |             名称              |           描述           |                             示例                             | 备注/状态 |
| :---: | :---------------------------: | :----------------------: | :----------------------------------------------------------: | --------- |
| Array |             Array             |         静态数组         |                 [Array](examples/array.cpp)                  |           |
|       |            Vector             |         动态数组         |                [Vector](examples/vector.cpp)                 |           |
| List  |     Embedded List      |        嵌入式链表        |             [EList](examples/embedded_list.cpp)              |           |
|       |   DLinkedList(Doubly Linked List)   |          双链表          |           [DLinkedList](examples/double_linked_list.cpp)           |           |
| Queue |    Deque(DoubleEndedQueue)    |         双端队列         |                 [Deque](examples/deque.cpp)                  |           |
|       |             Queue             |           队列           |                 [Queue](examples/queue.cpp)                  |           |
|       |         PriorityQueue         |   优先队列(Heap的别名)   |                                                              |           |
|       |                               |                          |                                                              |           |
| Stack |             Stack             |            栈            |                 [Stack](examples/stack.cpp)                  |           |
|       | XValueStack/MinStack/MaxStack | 最值栈/最小值栈/最大值栈 |        [XValueStack](examples/stack/xvalue_stack.cpp)        |           |
|       |                               |                          |                                                              |           |
|       |                               |                          |                                                              |           |
| Heap  |     Heap/MinHeap/MaxHeap      |   堆/最小值堆/最大值堆   | [Heap](examples/heap.cpp)/[MinHeap](examples/heap.cpp)/[MaxHeap](examples/heap.cpp) |           |
|       |                               |                          |                                                              |           |
| Tree  |           EBinaryTree            | 嵌入式二叉树 |                                                              |           |
|       |            BSTree / BinarySearchTree            |   二叉搜索树(默认less)   |          [BSTree](examples/binary_search_tree.cpp)           |           |

> 注: 功能开发中, 性能暂时未测试和优化



## 算法列表

> 范型算法

| 类别     |                             算法                             | 描述                       | 备注 |
| -------- | :----------------------------------------------------------: | -------------------------- | ---- |
| 通用     |         [for_each](examples/algorithms/for_each.cpp)         | 遍历                       |      |
|          |                                                              |                            |      |
|          |                                                              |                            |      |
| 堆(Heap) |       [Heap::build](examples/algorithms/heap_algo.cpp)       | 堆化                       |      |
|          | [Heap::sort / MinHeap::sort / MaxHeap::sort](examples/algorithms/heap_algo.cpp#L31) | 堆排序                     |      |
|          |                                                              |                            |      |
| 树(Tree) |      binary tree traversal(preorder/inorder/postorder)       | 二叉树遍历(前序/中序/后序) |      |
|          |     [BSTree::traversal](examples/binary_search_tree.cpp#L35)     | 二叉搜索树遍历             |      |





## 构建和运行示例

  ```
  curl -fsSL https://xmake.io/shget.text | bash # 安装 xmake
  xmake   # 编译/构建
  xmake r dstruct_vector # 运行 dstruct_vector 示例，更多细节请查看xmake.lua
  ```

## 谁在用?

- [**DSVisual**：](https://github.com/Sunrisepeak/DSVisual) 数据结构可视化组件库

![](https://github.com/Sunrisepeak/DSVisual/blob/main/docs/imgs/dsvisual_effect.readme.png)


## 其他

  - [Github](https://github.com/Sunrisepeak/DStruct)

