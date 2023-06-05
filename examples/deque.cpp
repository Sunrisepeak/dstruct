#include <iostream>

#include "dstruct.hpp"


int main() {

    std::cout << "\n\nTesting: " << __FILE__ << std::endl;

    dstruct::DoubleEndedQueue<int, 10> _deque; // 0. test cntor
    dstruct::Deque<int, 5> deque;

// test dynamic resize
    for (int i = 0; i < 20; i++) {
        deque.push_back(i);
        std::cout << "push back:  " << deque.size() << " " << deque.capacity() << std::endl;
        deque.push_front(i);
        std::cout << "push front: " << deque.size() << " " << deque.capacity() << std::endl;
    }

    for (int i = 0; i < 10; i++) {
        deque.pop_front();
        std::cout << "pop front: " << deque.size() << " " << deque.capacity() << std::endl;
        deque.pop_back();
        std::cout << "pop back:  " << deque.size() << " " << deque.capacity() << std::endl;

    }

    while (!deque.empty()) {
        deque.pop_front();
        std::cout << "pop front: " << deque.size() << " " << deque.capacity() << std::endl;
    }

    std::cout << std::endl;

    return 0;
}