#include <iostream>

#include "TestBase.hpp"

int main() {
    std::cout << "\nTesting: " << __FILE__;
    
    test::test_arr_destory();
    test::test_destory<dstruct::Vector<test::Destory>>();

    std::cout << "   pass" << std::endl;
}