// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#include <iostream>

#include "dstruct.hpp"


int main() {

    std::cout << "\nTesting: " << __FILE__;

    dstruct::MinStack<int> minStack;
    dstruct::MaxStack<int> _maxStack;
    dstruct::stack::XValueStack<int, dstruct::greater<int>> maxStack;

    for (int i = 0; i < 100; i++) {
        minStack.push(100 - i);
        maxStack.push(i);
        DSTRUCT_ASSERT(maxStack.getXValue() == i);
        DSTRUCT_ASSERT(minStack.getXValue() == 100 - i);
    }

    for (int i = 0; i < 100; i++) {
        int v1 = minStack.top(); minStack.pop();
        int v2 = maxStack.top(); maxStack.pop();
        //std::cout << v1 << "-" << v2 << std::endl;
        DSTRUCT_ASSERT(100 - v1 == v2);
    }

    DSTRUCT_ASSERT(minStack.empty());
    DSTRUCT_ASSERT(minStack.empty());

    std::cout << "   pass" << std::endl; 

    return 0;
}