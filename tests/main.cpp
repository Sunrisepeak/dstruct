#include <iostream>

#include "TestBase.hpp"

int main() {
    std::cout << "\nTesting: " << __FILE__;
    
    test::test_arr_destroy();
    test::test_destroy<dstruct::Vector<test::Destory>>();

    test::test_sma_allocator();

    std::cout << "   pass" << std::endl;
}