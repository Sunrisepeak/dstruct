#include <iostream>

#include "dstruct.hpp"


int main() {

    std::cout << "\n\nTesting: " << __FILE__ << std::endl;

    dstruct::DoubleEndedQueue<int, 10> _deque; // 0. test cntor
    dstruct::Deque<int, 8> deque;

    for (int i = 0; i < 20; i++) {
        deque.push_back(i);
        std::cout << deque.size() << " " << deque.capacity() << " / ";
        deque.push_front(i);
        std::cout << deque.size() << " " << deque.capacity() << std::endl;
    }

    std::cout << std::endl;

    return 0;
}