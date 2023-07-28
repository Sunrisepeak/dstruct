#include <iostream>

#include "dstruct.hpp"

int main() {

    std::cout << "\nTesting: " << __FILE__;

    dstruct::SinglyLinkedList<int> list(10, 2);
    dstruct::SLinkedList<double> list2(2, 1.1);

    DSTRUCT_ASSERT(list2.size() == 2);

    for (auto &v : list) { v = 3; }

    int cnt = 0;
    for (auto v : list) { cnt++; /* const iterator */ }
    DSTRUCT_ASSERT(list2.size() == cnt);

    while (!list.empty()) {
        list.pop_front();
    }

    DSTRUCT_ASSERT(list.size() == 0);

    for (int i = 0; i < 10; i++) {
        list.push(i);
        DSTRUCT_ASSERT(list.front() == i);
    }

    DSTRUCT_ASSERT(list.size() == 10);

    std::cout << "   pass" << std::endl;

    return 0;
}