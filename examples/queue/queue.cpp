// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#include <iostream>

#include <dstruct.hpp>


int main() {

    std::cout << "\nTesting: " << __FILE__;

    dstruct::Queue<int /*, dstruct::DoubleEndedQueue<T, 32> */ > queue1;
    dstruct::adapter::Queue<int, dstruct::DLinkedList<int>> queue2;

    for (int i = 0; i < 100; i++) {
        queue1.push(i);
        queue2.push(i);
    }

    for (int i = 0; i < 100; i++) {
        int v1 = queue1.front();
        int v2 = queue2.front();
        DSTRUCT_ASSERT(v1 == v2);
        queue1.pop();
        queue2.pop();
    }

    DSTRUCT_ASSERT(queue1.size() == 0);
    DSTRUCT_ASSERT(queue1.size() == queue2.size());

    std::cout << "   pass" << std::endl; 

    return 0;
}