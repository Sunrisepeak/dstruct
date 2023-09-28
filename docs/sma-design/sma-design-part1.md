## DStruct: 静态内存分配器(SMA) 设计与实现 - 简单分配器



### 0. 简介 - 简单分配器篇

- 基础概念: 内存、分配器、分配器规范
- 数据结构: 数组、内存块标记



### 1.内存的基本概念

> 逻辑上可以从字节数组的角度理解内存及内存块(这里不讨论具体内存物理结构和区分物理内存/虚拟内存)

![image](https://github.com/Sunrisepeak/DStruct/assets/38786181/e77e6ecf-1bbb-4cd8-b68f-58d6aec2165e)

上图简单描述了一个大小为 N个字节 的 内存/内存块,  同时我们也可把这块内存视为有N个字节元素的数组。图中对一个内存单元进行了展开, 它的的地址是1, 里面存储着二进制数据为 `0000 1011`, 而这个数据的解释权或者说意义是交给编译器和使用这个数据的程序员的。你可以认为它是数字12 或者 一个特殊ASCII码字符, 这个一般由编译器的 **类型系统** 去标识。下面由段代码来表示这样一块内存:

```cpp
// N = 1024 / 1K
char array[1024];
int main() {
    array[0] = 16;

    *(array + 1) = 0000_1011b;  //array[1] = 12;

    array[2] = 0;
    array[3] = 4;
    array[4] = 5;
    // ....
    array[1024 - 2] = 0;
    array[1024 - 1] = 33;
}
```

上述程序中的array标识了一块1024字节的内存块, 这块内存本质是存储着**无类型的01数据** , 编译器把这块内存标识为 **一个1024大小char型的数组类型**。

 ```cpp
 *(array + 1) = 0000_1011b;  //array[1] = 12;
 ```

表示我们可以通过直接修改这个地址指向的内存单元的数据, 也可以使用数组的方式去修改。

这让我们有了修改指定内存单元数据的手段, 而这内存单元存储的数据的意义 由 编译器去辅助标识 但最终的解释权归这个内存的所有者。简单的说就是:

**一块内存本质是0和1的数据, 而这串01数据表示的意义及使用规则由它的所有者制定, 而编译器的工作是辅助标识**



### 2. 分配器接口/规范

> 对于内存分配器的用户来说, 最关心的是分配和释放内存的接口, 这两个接口也是分配器最基础的接口。

#### 2.1 malloc/free

```cpp
#include <cmalloc>

int main() {
    void * memPtr = malloc(1);
    free(memPtr);
    return 0;
}
```

很多人都通过malloc/free 或 new/delete 去分配和释放过内存。代码描述如下:

```cpp
struct Allocator {
    static void * malloc(int size);
    static void free(void *ptr);
};
```

#### 2.2 DStruct中的AllocSpec规范

> AllocSpec(可视为分配器的Wrapper)规定了分配器的接口规范

```cpp
template <typename T, typename Alloc>
struct AllocSpec {
    static T *allocate(int n = 1) {
        return static_cast<T *>(Alloc::allocate(n * sizeof(T)));
    }
    static void deallocate(T *ptr, int n = 1) {
        Alloc::deallocate(ptr, n * sizeof(T));
    }
};
```

上面的分配器封装, 对分配器接口做了如下规范

```cpp
struct Alloc {
    static void * allocate(int bytes);
    static void deallocate(void *addr, int bytes);
};
```

#### 2.3 两种接口的直观对比

| 接口类型\分配和释放 | 分配接口         | 释放接口               |
| ------------------- | ---------------- | ---------------------- |
| malloc/free         | 需给出请求块大小 | **不用给出内存块大小** |
| allocate/deallocate | 需给出请求块大小 | 需给出请求块大小       |

最直观的就是`malloc/free`接口组在释放内存时候, 不需要给出释放的内存大小。如果内存块的大小不用用户记录了但需要**注意内块的大小的记录不会消失, 只是转移到分配器内部进行记录了**, 所以当请求一个N字节大小的内存时, 往往可能真正消耗的内存是大于N字节的。



### 3. 内存分配器的简单设计与实现 - malloc/free

> 有了1中对内存的解释, array对于我们来说就是一块**无类型**的内存, 下面我们就以它设计一个简单的内存分配器。

![image](https://github.com/Sunrisepeak/DStruct/assets/38786181/4ec8f826-d540-4bcd-bf2e-4eea55ad3501)

#### 3.1 内存块

> 为了简化分配, 我们定义一个固定大小的内存块结构, 用它来划分内存。
>
> 如果你了解过操作系统内核中的 内存页(一般4K大小)的概念, 在这里可以把这个功能等同理解。

```cpp
// Unused-Flag 0
using MemBlockFlag = unsigned long long;
struct MemBlock {
    MemBlockFlag flag; // flag area
    char mem[MEM_BLOCK_SIZE - sizeof(MemBlockFlag)]; // useable area
};
```

这个内存结构可分为**Head**和**有效内存**两个部分, Head中存储的是这块内存的状态和标志。例如flag标识这个内存块区域的状态:

- flag: 0 代表内存块空闲
- flag>0: 代表内存块被分配并标识用户请求的内存size



#### 3.2 内存区域初始化 - init

```cpp
static void init() {
    char *memPtr = array;
    while (memPtr + sizeof(MemBlock) < array + sizeof(array)) {
        auto mbPtr = (MemBlock *)memPtr;
        mbPtr->flag = 0;
        memPtr = memPtr + MEM_BLOCK_SIZE;
    }
}
```

把array数组标识的内存区域, 按MemBlock格式进行初始化对应的内存块为空闲状态



#### 3.3 内存分配接口 - malloc

```cpp
static void * malloc(int size) {
    bool allocateFailed = true;
    MemBlock *mbPtr = nullptr;
    for (int i = 0; i <= sizeof(array) - sizeof(MemBlock); i += sizeof(MemBlock)) {
        mbPtr = (MemBlock *)(array + i);
        if (mbPtr->flag == 0) {
            allocateFailed = false;
            break;
        }
    }
    if (allocateFailed) return nullptr;
    mbPtr->flag = size;
    return &(mbPtr->mem);
}
```

在malloc的实现中, 当有分配请求时, 通过一个简单的for循环遍历内存块并通过flag查询空闲状态 来选取要分配的内存块。

获取指定内存块后, 更新flag标志(这里用来记录用户请求的内存大小), 并把内存块中的有效区域返回给用户。

> **需要注意的是为了简洁的说明分配过程, 在这里并未处理一些 不在预期中的情况(例如: 当请求内存大于内存块大小时 或 使用者只请求了 4字节 而一个内存块有32字节 造成内存浪费的情况)**



#### 3.4 内存释放接口 - free

```cpp
static void free(void *ptr) {
    auto mbPtr = (MemBlock *)((char *)ptr - sizeof(MemBlockFlag));
    mbPtr->flag = 0;
}
```

内存的释放就相对简单很多, 把指向**有效内存**的指针向上偏移到MemBlock内存块的位置, 然后再进行初始化该内存块的状态/flag。



#### 3.5 SimpleAllocate & 测试完整代码

```cpp
#include <iostream>
#include <cassert>

#include <dstruct.hpp>

/*

g++ dstruct/simple_allocator.cpp -I ../DStruct && ./a.out
g++ dstruct/simple_allocator.cpp -DDEBUG -I ../DStruct && ./a.out

*/

//#define DEBUG

#define LOGI(...) printf("%s: ", __func__); printf(__VA_ARGS__); printf("\n")

#ifdef DEBUG
#define LOGD(...) LOGI(__VA_ARGS__)
#else
#define LOGD(...)
#endif

char array[1024] { 0 }; // 1k memory

// mem-flag: 0 free, other allocated
#define MEM_BLOCK_SIZE 32
struct SimpleAllocator {

    // Unused-Flag 0
    using MemBlockFlag = unsigned long long;

    struct MemBlock {
        MemBlockFlag flag; // flag area
        char mem[MEM_BLOCK_SIZE - sizeof(MemBlockFlag)]; // useable area
    };

    static void init() {
        LOGI("init allocate: memory address %p, size %ld", array, sizeof(array));
        char *memPtr = array;
        while (memPtr + sizeof(MemBlock) < array + sizeof(array)) {
            auto mbPtr = (MemBlock *)memPtr;
            mbPtr->flag = 0;
            memPtr = memPtr + MEM_BLOCK_SIZE;
            LOGD("address %p, block-size %ld", mbPtr, sizeof(MemBlock));
        }
    }

    static void * malloc(int size) {

        assert(size <= sizeof(MemBlock) - sizeof(MemBlockFlag));

        bool allocateFailed = true;
        MemBlock *mbPtr = nullptr;

        for (int i = 0; i <= sizeof(array) - sizeof(MemBlock); i += sizeof(MemBlock)) {
            mbPtr = (MemBlock *)(array + i);
            if (mbPtr->flag == 0) {
                allocateFailed = false;
                break;
            }
        }

        if (allocateFailed) {
            LOGI("request size %d, memory allocate failed...", size);
            return nullptr;
        }

        LOGD("addr %p, request size %d", mbPtr, size);

        mbPtr->flag = size;

        return &(mbPtr->mem);
    }

    static void free(void *ptr) {
        auto mbPtr = (MemBlock *)((char *)ptr - sizeof(MemBlockFlag));

        LOGD("addr %p, size %lld, block-size %d", mbPtr, mbPtr->flag, MEM_BLOCK_SIZE);

        assert(
            array <= (char *)mbPtr &&
            (char *)mbPtr <= array + sizeof(array) - sizeof(MemBlock) &&
            "memory free failed - range"
        );

        assert(mbPtr->flag != 0 && "memory free failed - flag error(double free)");

        mbPtr->flag = 0;
    }
};

struct RGB {
    char r;
    short g;
    char b;
};

// test SimpleAllocator
int main() {

    dstruct::Vector<int *> ptrVec;

    SimpleAllocator::init();

    for (int i = 0; i < 50; i++) {
        auto intPtr = (int *) SimpleAllocator::malloc(sizeof(int));

        if (intPtr == nullptr)
            break;

        *intPtr = i;
        ptrVec.push_back(intPtr);
    }

    RGB *rgbPtr = (RGB *) SimpleAllocator::malloc(sizeof(RGB));

    if (rgbPtr == nullptr) {
        int *intPtr = ptrVec.back();
        LOGI("free %d, addr %p", *intPtr, intPtr);
        SimpleAllocator::free(ptrVec.back());
        ptrVec.pop_back();
        rgbPtr = (RGB *) SimpleAllocator::malloc(sizeof(RGB));
    }

    rgbPtr->r = 1;
    rgbPtr->g = 2;
    rgbPtr->b = 3;

    LOGI("rgb: (%d, %d, %d)", rgbPtr->r, rgbPtr->g, rgbPtr->b);

    for (int i = 0; i < ptrVec.size(); i++) {
        SimpleAllocator::free(ptrVec[i]);
    }

// failed test set
    //SimpleAllocator::malloc(25); // size limit
    //SimpleAllocator::free(ptrVec[0]); // test double free

    return 0;
}
```



#### 3.6 测试log

**默认分配与释放测试**

```bash
speak@speak-pc:~/workspace/github/HelloWorld$ g++ dstruct/simple_allocator.cpp -I ../DStruct && ./a.out
init: init allocate: memory address 0x560f30f41040, size 1024
malloc: request size 4, memory allocate failed...
malloc: request size 6, memory allocate failed...
main: free 31, addr 0x560f30f41428
main: rgb: (1, 2, 3)
```

**打开debug log 进行测试**

```cpp
speak@speak-pc:~/workspace/github/HelloWorld$ g++ dstruct/simple_allocator.cpp -DDEBUG -I ../DStruct && ./a.out
init: init allocate: memory address 0x55665776a040, size 1024
init: address 0x55665776a040, block-size 32
init: address 0x55665776a060, block-size 32
init: address 0x55665776a080, block-size 32
init: address 0x55665776a0a0, block-size 32
init: address 0x55665776a0c0, block-size 32
init: address 0x55665776a0e0, block-size 32
init: address 0x55665776a100, block-size 32
init: address 0x55665776a120, block-size 32
init: address 0x55665776a140, block-size 32
init: address 0x55665776a160, block-size 32
init: address 0x55665776a180, block-size 32
init: address 0x55665776a1a0, block-size 32
init: address 0x55665776a1c0, block-size 32
init: address 0x55665776a1e0, block-size 32
init: address 0x55665776a200, block-size 32
init: address 0x55665776a220, block-size 32
init: address 0x55665776a240, block-size 32
init: address 0x55665776a260, block-size 32
init: address 0x55665776a280, block-size 32
init: address 0x55665776a2a0, block-size 32
init: address 0x55665776a2c0, block-size 32
init: address 0x55665776a2e0, block-size 32
init: address 0x55665776a300, block-size 32
init: address 0x55665776a320, block-size 32
init: address 0x55665776a340, block-size 32
init: address 0x55665776a360, block-size 32
init: address 0x55665776a380, block-size 32
init: address 0x55665776a3a0, block-size 32
init: address 0x55665776a3c0, block-size 32
init: address 0x55665776a3e0, block-size 32
init: address 0x55665776a400, block-size 32
malloc: addr 0x55665776a040, request size 4
malloc: addr 0x55665776a060, request size 4
malloc: addr 0x55665776a080, request size 4
malloc: addr 0x55665776a0a0, request size 4
malloc: addr 0x55665776a0c0, request size 4
malloc: addr 0x55665776a0e0, request size 4
malloc: addr 0x55665776a100, request size 4
malloc: addr 0x55665776a120, request size 4
malloc: addr 0x55665776a140, request size 4
malloc: addr 0x55665776a160, request size 4
malloc: addr 0x55665776a180, request size 4
malloc: addr 0x55665776a1a0, request size 4
malloc: addr 0x55665776a1c0, request size 4
malloc: addr 0x55665776a1e0, request size 4
malloc: addr 0x55665776a200, request size 4
malloc: addr 0x55665776a220, request size 4
malloc: addr 0x55665776a240, request size 4
malloc: addr 0x55665776a260, request size 4
malloc: addr 0x55665776a280, request size 4
malloc: addr 0x55665776a2a0, request size 4
malloc: addr 0x55665776a2c0, request size 4
malloc: addr 0x55665776a2e0, request size 4
malloc: addr 0x55665776a300, request size 4
malloc: addr 0x55665776a320, request size 4
malloc: addr 0x55665776a340, request size 4
malloc: addr 0x55665776a360, request size 4
malloc: addr 0x55665776a380, request size 4
malloc: addr 0x55665776a3a0, request size 4
malloc: addr 0x55665776a3c0, request size 4
malloc: addr 0x55665776a3e0, request size 4
malloc: addr 0x55665776a400, request size 4
malloc: addr 0x55665776a420, request size 4
malloc: request size 4, memory allocate failed...
malloc: request size 6, memory allocate failed...
main: free 31, addr 0x55665776a428
free: addr 0x55665776a420, size 4, block-size 32
malloc: addr 0x55665776a420, request size 6
main: rgb: (1, 2, 3)
free: addr 0x55665776a040, size 4, block-size 32
free: addr 0x55665776a060, size 4, block-size 32
free: addr 0x55665776a080, size 4, block-size 32
free: addr 0x55665776a0a0, size 4, block-size 32
free: addr 0x55665776a0c0, size 4, block-size 32
free: addr 0x55665776a0e0, size 4, block-size 32
free: addr 0x55665776a100, size 4, block-size 32
free: addr 0x55665776a120, size 4, block-size 32
free: addr 0x55665776a140, size 4, block-size 32
free: addr 0x55665776a160, size 4, block-size 32
free: addr 0x55665776a180, size 4, block-size 32
free: addr 0x55665776a1a0, size 4, block-size 32
free: addr 0x55665776a1c0, size 4, block-size 32
free: addr 0x55665776a1e0, size 4, block-size 32
free: addr 0x55665776a200, size 4, block-size 32
free: addr 0x55665776a220, size 4, block-size 32
free: addr 0x55665776a240, size 4, block-size 32
free: addr 0x55665776a260, size 4, block-size 32
free: addr 0x55665776a280, size 4, block-size 32
free: addr 0x55665776a2a0, size 4, block-size 32
free: addr 0x55665776a2c0, size 4, block-size 32
free: addr 0x55665776a2e0, size 4, block-size 32
free: addr 0x55665776a300, size 4, block-size 32
free: addr 0x55665776a320, size 4, block-size 32
free: addr 0x55665776a340, size 4, block-size 32
free: addr 0x55665776a360, size 4, block-size 32
free: addr 0x55665776a380, size 4, block-size 32
free: addr 0x55665776a3a0, size 4, block-size 32
free: addr 0x55665776a3c0, size 4, block-size 32
free: addr 0x55665776a3e0, size 4, block-size 32
free: addr 0x55665776a400, size 4, block-size 32
```

**分配失败测试**

```bash
speak@speak-pc:~/workspace/github/HelloWorld$ g++ dstruct/simple_allocator.cpp -I ../DStruct && ./a.out
init: init allocate: memory address 0x55db86e65040, size 1024
malloc: request size 4, memory allocate failed...
malloc: request size 6, memory allocate failed...
main: free 31, addr 0x55db86e65428
main: rgb: (1, 2, 3)
a.out: dstruct/simple_allocator.cpp:49: static void* SimpleAllocator::malloc(int): Assertion `size <= sizeof(MemBlock) - sizeof(MemBlockFlag)' failed.
Aborted (core dumped)
```



### 4. SimpleAllocator的一些问题及思考

#### 4.1 问题

- **内存区域问题**: 管理的内存区域array暴露在外部
- **分配大小限制:** 分配的内存大小不能大于固定的内存块中有效内存的大小(`MEM_BLOCK_SIZE - sizeof(MemBlockFlag)`)
- **块内内存浪费:** 但请求的内存小于一个MemBlock的有效载荷时, 出现块内未使用的内存碎片
- **分配速度慢:** 每次都需要从头进行内存块的状态查询 **平均时间复杂度O(N)**
- ....



#### 4.2 思考

- 一个内存块的状态标记 是否一定要额外占用内存, 能不能使内存块的有效载荷为100%?
- 能否把空闲块和已被分配块 分开管理, 或 如何更巧妙的管理空闲内存?
- 不使用固定内存块会怎么样? 如果使用有没有更有效的方法避免过多的内存浪费?



本片文章并没有太多介绍内存管理与分配策略相关的技巧, 主要的目的是介绍分配器的基础概念(内存/接口), 可以简单概括为 **一块内存 + 一个管理/分配策略  = 分配器 **, 而并不特指 是什么内存 和 什么管理/分配策略



**后面的文章将对上面提到的问题和思考进一步讨论, 并尝试解决或在他们之前取一个平衡**



### 5. Other

[DStruct: 静态内存分配器(SMA) 设计与实现 - 目录](https://github.com/Sunrisepeak/DStruct/tree/main/docs/sma-design)

[DStruct项目地址](https://github.com/Sunrisepeak/DStruct)

[SMA静态内存分配器的源码](https://github.com/Sunrisepeak/DStruct/blob/main/memory/StaticMemAllocator.hpp)

[测试代码](https://github.com/Sunrisepeak/HelloWorld/blob/main/dstruct/simple_allocator.cpp)
