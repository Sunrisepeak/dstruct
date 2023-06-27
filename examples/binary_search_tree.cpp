#include <iostream>

#include "dstruct.hpp"


int main() {

    std::cout << "\nTesting: " << __FILE__;

    dstruct::BSTree<int> bst;

    for (int i = 0; i < 10; i++) {
        DSTRUCT_ASSERT(i == bst.size());
        bst.insert(i);
    }

    for (int i = 0; i < 10; i++) {
        bst.erase(i);
    }

    DSTRUCT_ASSERT(bst.empty());

    std::cout << "   pass" << std::endl;

    return 0;
}