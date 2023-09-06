# DStruct

DStruct is a portable and structurally simple data structure template library.


| [Features](#Features) - [Usage](#Usage) - [Core](#Core) - [Who's Using It?](#Whos using?) - [Other](#Other) - [中文](README.zh.md) |
| ------------------------------------------------------------ |
| Design Philosophy - Interface Documentation - Static Memory Allocator (SMA) - Porting Guide - Related Videos |

---


## Features

- Easily portable, no dependent std
- Easy to use, like std-lib style
- Easy to learn, customize, and improve
- Provides custom allocators interface
- Supports bare-metal/small-memory devices
- Provides simple static memory management/allocator - SMA
- Modern C++/generic structures


## Usage

### 1. Source Code and Configuration

- Download the source code to your local machine.
- Add the root directory of the DStruct library to your include path.
- Customize allocator & implement the dstruct-port.h interface - **optional**

### 2. Code Example - Dynamic Memory

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

### 3. Code Example - Static Memory

> Use static memory SMA, support no memory management environment (such as: bare metal)

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

**Note: Static data structures are defined in the `dstruct::smemory` space, and interfaces and usage are the same as the data structures supported by dynamic memory**


## Core

```cpp
.
├── algorithm.hpp
├── common.hpp
├── ds
│   ├── array
│   │   ├── Array.hpp
│   │   └── Vector.hpp
│   ├── Heap.hpp
│   ├── linked-list
│   │   ├── DoublyLinkedList.hpp
│   │   ├── EmbeddedList.hpp
│   │   ├── LinkedList.hpp
│   │   └── SinglyLinkedList.hpp
│   ├── queue
│   │   ├── DoubleEndedQueue.hpp
│   │   └── Queue.hpp
│   ├── stack
│   │   ├── Stack.hpp
│   │   └── XValueStack.hpp
│   └── tree
│       ├── BinarySearchTree.hpp
│       ├── BinaryTree.hpp
│       ├── EmbeddedBinaryTree.hpp
│       └── tree-base.hpp
├── Iterator.hpp
├── StaticMemAllocator.hpp
└── types.hpp
```


## Whos using?

- [**DSVisual**：](https://github.com/Sunrisepeak/DSVisual) Data structure visualization library

![](https://github.com/Sunrisepeak/DSVisual/blob/main/docs/imgs/dsvisual_effect.readme.png)

- [**KHistory**：](https://github.com/Sunrisepeak/KHistory) Cross-platform key detection/history visualization tool

![](https://github.com/Sunrisepeak/KHistory/blob/main/docs/imgs/khistory-gamepad.demo.gif)


## Other

  - [Github](https://github.com/Sunrisepeak)
  - [DStruct](https://github.com/Sunrisepeak/DStruct)