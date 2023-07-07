#ifndef __TEST_BASE_HPP__DSTRUCT
#define __TEST_BASE_HPP__DSTRUCT

//#include <iostream>
#include <dstruct.hpp>

#define ENABLE_LOG 0
#define TEST_LOG(...) \
if (ENABLE_LOG) { \
    printf ("\033[32mDStruct LOG [I]: \t%s:  ", __func__); \
    printf ("%s \033[0m\n", __VA_ARGS__); \
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
void test_destory() {
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

void test_arr_destory() {

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

}

#endif