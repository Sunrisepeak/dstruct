#include <iostream>

#include "TestBase.hpp"

int main() {
    std::cout << "\nTesting: " << __FILE__;
    
    test::test_arr_destory();

    std::cout << "   pass" << std::endl;
}