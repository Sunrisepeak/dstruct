#include <iostream>

#include "dstruct.hpp"

int main() {

    std::cout << "\n\nTesting: " << __FILE__ << std::endl;

    dstruct::DoubleLinkedList<int> list(2, 10);

    for (auto v : list) {     // 2. test const / range-for
        std::cout << v << " " << list.size() << std::endl;
    }

    std::cout << std::endl;

    while (!list.empty()) {
        std::cout << list.back() << " " << list.size() << std::endl;
        list.pop_back();
    }

    std::cout << std::endl;

    for (int i = 0; i < 10; i++) {
        list.push_back(i);
        std::cout << list.back() << " " << list.size() << std::endl;
    }

    return 0;
}