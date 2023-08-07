#ifndef __TEST_BASE_HPP__DSTRUCT
#define __TEST_BASE_HPP__DSTRUCT

//#include <iostream>
#include <dstruct.hpp>

#define ENABLE_LOG 0
#define TEST_LOG(...) \
if (ENABLE_LOG) { \
    printf("\033[32mDStruct LOG [I]: \t%s:  ", __func__); \
    printf(__VA_ARGS__); \
    printf("\033[0m\n"); \
}

namespace test {

struct Destory {
public:
    Destory() : __mCnt { 1 } {
        TEST_LOG("Destory");
    }

    Destory(const Destory &des) : __mCnt { des.__mCnt } {
        TEST_LOG("Copy Destory");
    }

    ~Destory() {
        __mCnt--;
        DSTRUCT_ASSERT(__mCnt == 0);
        TEST_LOG("~Destory");
    }
private:
    int __mCnt;
};

template <typename DStruct>
static void test_destory() {
    DStruct ds;
    Destory des;
    int testNums = 10;

    for(int i = 0; i < testNums; i++) {
        DSTRUCT_ASSERT(ds.size() == i);
        ds.push(des);
    }

    for(int i = testNums; i > 0; i--) {
        DSTRUCT_ASSERT(ds.size() == i);
        ds.pop();
    }

    DSTRUCT_ASSERT(ds.empty());
}

static void test_arr_destory() {

    { // test auto
        dstruct::Array<Destory, 10> ds1;
        dstruct::Array<Destory, 5> ds2(Destory());
    }

    //std::cout << std::endl;
    Destory des;
    //std::cout << std::endl;

    {
        dstruct::Array<Destory, 20> ds;
        for(int i = 10; i < 20; i++) {
            ds[i] = des;
        }
    }

    //std::cout << std::endl;
}

static void test_sma_allocator() {
    using MyMemAlloc = dstruct::StaticMemAllocator<1024, 1024>; // define a 1024byte static allocator

// allocate/release memory - test

    int memSize = 1024;
    dstruct::Array<void *, 8> memArr;
    for (int i = 0; i < memArr.size(); i++) {
        memArr[i] = MyMemAlloc::allocate(memSize /= 2);
        if (i > 0) {
            TEST_LOG("%p %p", memArr[i - 1], memArr[i]);
            DSTRUCT_ASSERT(memArr[i - 1] + memSize * 2 == memArr[i]);
        }
    }

    // release all memory
    memSize = 1024;
    for (int i = 0; i < memArr.size(); i++) {
        MyMemAlloc::deallocate(memArr[i], memSize /= 2);
    }

// memory merge - test1

    // try to alloc a 1024byte mem-block, it should get a nullptr(alloc failed)
    // exist memory fragment
    auto *mem1024 = MyMemAlloc::allocate(1024);
    if (mem1024 == nullptr) {
        MyMemAlloc::memory_merge();
    }
 
    DSTRUCT_ASSERT(mem1024 == nullptr);
    
    MEM_VERIFY(mem1024 = MyMemAlloc::allocate(1024)); // ok - only debug

    DSTRUCT_ASSERT(MyMemAlloc::deallocate(mem1024, 1024) == true);

// memory merge - test2
    {   //    list-index    0  3   7   63   1   31   15   0
        int memAllocSeq[8] {8, 32, 64, 512, 16, 256, 128, 8};

        for (int i = 0; i < memArr.size(); i++) {
            memArr[i] = MyMemAlloc::allocate(memAllocSeq[i]);
            if (i > 0) {
                TEST_LOG("%p %p", memArr[i - 1], memArr[i]);
                DSTRUCT_ASSERT(memArr[i - 1] + memAllocSeq[i - 1] == memArr[i]);
            }
        }

        for (int i = 0; i < memArr.size(); i++) {
            MyMemAlloc::deallocate(memArr[i], memAllocSeq[i]);
        }

        DSTRUCT_ASSERT(MyMemAlloc::max_free_mblock_size() == 512);
        TEST_LOG(" max_free_mblock_size - %d", MyMemAlloc::max_free_mblock_size());
        MyMemAlloc::memory_merge();
        TEST_LOG(" max_free_mblock_size - %d", MyMemAlloc::max_free_mblock_size());
        DSTRUCT_ASSERT(MyMemAlloc::max_free_mblock_size() == 1024);
    }
}

}

#endif