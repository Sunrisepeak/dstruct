#include <iostream>

#include "dstruct.hpp"

int main() {

    std::cout << "\nTesting: " << __FILE__;

    dstruct::DoublyLinkedlist<int> list(2, 10);
    dstruct::DLinkedList<double> list2(1.1, 2);

    DSTRUCT_ASSERT(list2.size() == 2);

    for (auto &v : list) {     // range-for
        //std::cout << v << " " << list.size() << std::endl;
        v = 3;
    }

    DSTRUCT_ASSERT(list.back() == 3);

    while (!list.empty()) {
        //std::cout << list.back() << " " << list.size() << std::endl;
        list.pop_back();
    }

    DSTRUCT_ASSERT(list.size() == 0);

    for (int i = 0; i < 10; i++) {
        list.push_back(i);
        DSTRUCT_ASSERT(list.back() == i);
        //std::cout << list.back() << " " << list.size() << std::endl;
    }

    DSTRUCT_ASSERT(list.size() == 10);

    std::cout << "   pass" << std::endl;

    return 0;
}