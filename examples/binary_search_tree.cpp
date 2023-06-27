#include <iostream>

#include "dstruct.hpp"


int main() {

    std::cout << "\nTesting: " << __FILE__;

    {
        dstruct::BSTree<int> bst;

        for (int i = 0; i < 10; i++) {
            DSTRUCT_ASSERT(2 * i == bst.size());
            bst.insert(i);
            bst.insert(100 - i);
        }

        for (int i = 0; i < 10; i++) {
            bst.erase(i);
            bst.erase(100 - i);
        }

        DSTRUCT_ASSERT(bst.empty());

    }

    {
        int arr[5] { 3, 4, 2, 5, 1 };
        dstruct::BSTree<int> bst(arr, arr + sizeof(arr));
        DSTRUCT_ASSERT(bst.size() == sizeof(arr));
    }

    std::cout << "   pass" << std::endl;

    return 0;
}