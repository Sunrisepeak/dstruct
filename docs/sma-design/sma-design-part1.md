## DStruct: 静态内存分配器(SMA) 设计与实现 - 简单分配器

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



### 2. 分配器 及 分配器规范

> 有了上面对内存的认识, array 对于我们来说就是一个1024字节大小的内存块, 怎么用它就由我们说了算。

#### 2.1 分配器简介

```cpp
#include <cmalloc>

int main() {
    void * memPtr = malloc(1);
    free(memPtr);
    return 0;
}
```

很多人都通过malloc/free 或 new/delete 去分配和释放过内存。由分配和释放可以组成如下最简分配器模型:

```cpp
struct Allocator {
    static void * malloc(int size);
    static void free(void *ptr);
};
```

下面是一个字节分配器的实现及测试

```cpp
#include <iostream>
#include <cassert>

#include <dstruct.hpp>

/*

g++ dstruct/byte_allocator.cpp -I ../DStruct && ./a.out

*/

char array[1024] { 0 }; // 1k memory

// mem-flag: 0 free, other allocated
struct ByteAllocator {
    static void * malloc(int size) {

        assert(size == 1);

        void *memPtr = nullptr;
        for (int i = 0; i < 1024; i++) {
            if (array[i] == 0) {
                memPtr = array + i;
                break;
            }
        }

        assert(memPtr != nullptr && "memory allocate failed.");

        printf("malloc: addr %p\n", memPtr);

        return memPtr;
    }

    static void free(void *ptr) {
        auto cPtr = (char *)ptr;

        printf("free: addr %p, %X\n", cPtr, *cPtr);

        assert(array <= cPtr && cPtr < array + 1024 && "memory free failed - range");
        assert(*cPtr != 0 && "memory free failed - flag error");

        *(cPtr) = 0; // release - reset flag
    }
};

// test ByteAllocator
int main() {

    dstruct::Vector<char *> cVec;

    printf("\n[malloc/free]\n");
    for (int i = 0; i < 1024; i++) {
        char * cPtr = (char *) ByteAllocator::malloc(sizeof(char));

        *cPtr = i % 127 + 1; // ascii

        if (*cPtr == 'M')
            ByteAllocator::free(cPtr);
        else
            cVec.push_back(cPtr);
    }

    printf("\n[free]: vec size %lld\n", cVec.size());
    for (int i = 0; i < cVec.size(); i++) {
        ByteAllocator::free(cVec.back());
        cVec.pop_back();
    }

    return 0;
}

```



....
