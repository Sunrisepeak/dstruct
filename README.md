# DStruct

DStruct 是一个易于移植且结构简洁的数据结构模板库

| [特性](#特性) - [用法](#用法) -  [数据结构列表](#数据结构列表) - [算法列表](#算法列表) - [谁在用?](#谁在用) - [其他](#其他) - [ English](README.en.md) |
| ------------------------------------------------------------ |
| 设计思想 - 接口文档 - 静态内存分配器(SMA) - 移植介绍 - 相关视频 |

---

## 特性

- 易于移植，不依赖标准库(std)和其它三方库
- 易于使用（数据结构和算法分离设计，统一接口，std标准库风格）
- 易于学习/定制和改进（结构简洁且只有头文件）
- 提供静态数据结构和自定义数据结构分配器功能
- 支持裸机/小内存设备, 提供简单的静态内存管理/分配器SMA
- 现代 C++ / 泛型结构，包含多样丰富的数据结构


## 用法

### 1. 源码及配置

- 下载源码到本地
- 添加DStruct库的根目录到你的include的路径
- 自定义分配器&实现dstruct-port.h接口 - **可选**

### 2. 代码用例 - 动态内存

```cpp
#include <iostream>
#include "dstruct.hpp"

int main() {
    dstruct::Array<int, 10> arr(2);

    decltype(arr)::ValueType val = 6;
    arr[0] = arr[-1] = val;

    for (int i = 0; i < arr.size(); i++) {
        std::cout << arr[-(i + 1)] << " : " << arr[i] << std::endl;
    }
    return 0;
}
```

### 3. 代码用例 - 静态内存

> 使用静态内存SMA, 支持无内存管理环境(如: 裸机)

```cpp
#include <dstruct.hpp>

int main() {
    //dstruct::Vector<int> dVec;
    dstruct::smemory::Vector<int> sVec;

    for (int i = 0; i < 10; i++) {
        sVec.push_back(i);
    }

    int i = 0;
    for (auto v : sVec) {
        DSTRUCT_ASSERT(v == i++);
    }

    while (!sVec.empty()) {
        sVec.pop_back();
    }

    return 0;
}
```

### 3. 代码用例 - 静态内存

> 使用静态内存SMA, 支持无内存管理环境(如: 裸机)

```cpp
#include <dstruct.hpp>

int main() {
    //dstruct::Vector<int> dVec;
    dstruct::smemory::Vector<int> sVec;

    for (int i = 0; i < 10; i++) {
        sVec.push_back(i);
    }

    for (auto v : sVec)
        DSTRUCT_ASSERT(v == i++);

    while (!sVec.empty()) {
        sVec.pop_back();
    }

    return 0;
}
```



**注: 静态数据结构定义在`dstruct::smemory`空间里, 其他接口及用法同动态内存支持的数据结构一样**




**注: 静态数据结构定义在`dstruct::smemory`空间里, 其他接口及用法同动态内存支持的数据结构一样**



## 数据结构列表

| 类别  |             名称/具体实现              |           描述           |                             示例                             | 备注/状态 |
| :---: | :---------------------------: | :----------------------: | :----------------------------------------------------------: | --------- |
| Array |             [Array](https://github.com/Sunrisepeak/DStruct/tree/main/core/ds/array/Array.hpp)             |         静态数组         |                 [Array](https://github.com/Sunrisepeak/DStruct/tree/main/examples/array/array.cpp)                  |           |
|       |            [Vector](https://github.com/Sunrisepeak/DStruct/tree/main/core/ds/array/Vector.hpp)             |         动态数组         |                [Vector](https://github.com/Sunrisepeak/DStruct/tree/main/examples/array/vector.cpp)                 |           |
| List  |     [Embedded List](https://github.com/Sunrisepeak/DStruct/tree/main/core/ds/linked-list/EmbeddedList.hpp)      |        嵌入式链表        |             [EList](https://github.com/Sunrisepeak/DStruct/tree/main/examples/linked-list/embedded_list.cpp)              |           |
|       |   [SLinkedList(Singly Linked List)](https://github.com/Sunrisepeak/DStruct/tree/main/core/ds/linked-list/SinglyLinkedList.hpp)   |          单链表          |           [SLinkedList](https://github.com/Sunrisepeak/DStruct/tree/main/examples/linked-list/single_linked_list.cpp)           |           |
|       |   [DLinkedList(Doubly Linked List)](https://github.com/Sunrisepeak/DStruct/tree/main/core/ds/linked-list/DoublyLinkedList.hpp)   |          双链表          |           [DLinkedList](https://github.com/Sunrisepeak/DStruct/tree/main/examples/linked-list/double_linked_list.cpp)           |           |
| Queue |             [Queue(adapter)](https://github.com/Sunrisepeak/DStruct/tree/main/core/ds/queue/Queue.hpp)             |           队列适配器           |                 [Queue](https://github.com/Sunrisepeak/DStruct/tree/main/examples/queue/queue.cpp)                  |           |
|       |    [Deque(DoubleEndedQueue)](https://github.com/Sunrisepeak/DStruct/tree/main/core/ds/queue/DoubleEndedQueue.hpp)    |         双端队列         |                 [Deque](https://github.com/Sunrisepeak/DStruct/tree/main/examples/queue/deque.cpp)                  |           |
|       |         PriorityQueue         |   优先队列(Heap的别名)   |    [PriorityQueue](https://github.com/Sunrisepeak/DStruct/tree/main/examples/heap.cpp)   |           |
|       |                               |                          |                                                              |           |
| Stack |             [Stack(adapter)](https://github.com/Sunrisepeak/DStruct/tree/main/core/ds/stack/Stack.hpp)             |            栈适配器            |                 [Stack](https://github.com/Sunrisepeak/DStruct/tree/main/examples/stack/stack.cpp)                  |           |
|       | [XValueStack/MinStack/MaxStack](https://github.com/Sunrisepeak/DStruct/tree/main/core/ds/stack/XValueStack.hpp) | 最值栈/最小值栈/最大值栈 |        [XValueStack](https://github.com/Sunrisepeak/DStruct/tree/main/examples/stack/xvalue_stack.cpp)        |           |
|       |                               |                          |                                                              |           |
|       |                               |                          |                                                              |           |
| Heap  |     [Heap/MinHeap/MaxHeap](https://github.com/Sunrisepeak/DStruct/tree/main/core/ds/Heap.hpp)      |   堆/最小值堆/最大值堆   | [Heap / MinHeap/ MaxHeap](https://github.com/Sunrisepeak/DStruct/tree/main/examples/heap.cpp) |           |
|       |                               |                          |                                                              |           |
| Tree  |           [EBinaryTree](https://github.com/Sunrisepeak/DStruct/tree/main/core/ds/tree/EmbeddedBinaryTree.hpp)            | 嵌入式二叉树 |                                                              |           |
|       |            [BSTree / BinarySearchTree](https://github.com/Sunrisepeak/DStruct/tree/main/core/ds/tree/BinarySearchTree.hpp)            |   二叉搜索树(默认less)   |          [BSTree](https://github.com/Sunrisepeak/DStruct/tree/main/examples/binary_search_tree.cpp)           |           |

> 注: 功能开发中, 性能暂时未测试和优化



## 算法列表

> 范型算法

| 类别     |                             算法                             | 描述                       | 备注 |
| -------- | :----------------------------------------------------------: | -------------------------- | ---- |
| 通用     |         [for_each](https://github.com/Sunrisepeak/DStruct/tree/main/examples/algorithms/for_each.cpp)         | 遍历                       |      |
|          |                                                              |                            |      |
|          |                                                              |                            |      |
| 堆(Heap) |       [Heap::build](https://github.com/Sunrisepeak/DStruct/tree/main/examples/algorithms/heap_algo.cpp)       | 堆化                       |      |
|          | [Heap::sort / MinHeap::sort / MaxHeap::sort](https://github.com/Sunrisepeak/DStruct/tree/main/examples/algorithms/heap_algo.cpp#L31) | 堆排序                     |      |
|          |                                                              |                            |      |
| 树(Tree) |      binary tree traversal(preorder/inorder/postorder)       | 二叉树遍历(前序/中序/后序) |      |
|          |     [BSTree::traversal](https://github.com/Sunrisepeak/DStruct/tree/main/examples/binary_search_tree.cpp#L35)     | 二叉搜索树遍历             |      |




## 谁在用?

- [**DSVisual**：](https://github.com/Sunrisepeak/DSVisual) 数据结构可视化组件库

![](https://github.com/Sunrisepeak/DSVisual/blob/main/docs/imgs/dsvisual_effect.readme.png)

- [**KHistory**：](https://github.com/Sunrisepeak/KHistory) 跨平台的按键检测/历史记录工具

![](https://github.com/Sunrisepeak/KHistory/blob/main/docs/imgs/khistory-gamepad.demo.gif)

## 其他

  - [Github](https://github.com/Sunrisepeak)
  - [项目主页](https://github.com/Sunrisepeak/DStruct)

