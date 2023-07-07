#ifndef __TEST_BASE_HPP__DSTRUCT
#define __TEST_BASE_HPP__DSTRUCT

//#include <iostream>
#include <dstruct.hpp>

namespace test {

class Destory {
public:
    Destory() : __mCnt { 1 } {
        //std::cout << "Destory:" << this << std::endl;
    }

    ~Destory() {
        __mCnt--;
        DSTRUCT_ASSERT(__mCnt == 0);
        //std::cout << "~Destory:" << this << std::endl;
    }
private:
    int __mCnt;
};

template <typename DStruct>
void test_destory() {
    DStruct ds;
    for(int i = 0; i < 100; i++) {
        ds.push(Destory());
    }
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