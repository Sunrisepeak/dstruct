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

    {
        dstruct::AVLTree<int, dstruct::less<int>, dstruct::Alloc> avlTree;

        for (int i = 100; i >= 0; i--) { // Test: R-Rotate
            avlTree.insert(i);
        }
        for (int i = 101; i <= 200; i++) { // Test: L-Rotate
            avlTree.insert(i);
        }
        for (int i = 1; i <= 100; i++) {
            avlTree.insert(500 + i);
            avlTree.insert(500 - i);
        }

        std::cout << avlTree.height() << std::endl;
        DSTRUCT_ASSERT(avlTree.height() <= 9 /* 2^9 = 512*/);
    }

    std::cout << "   pass" << std::endl;

    return 0;
}