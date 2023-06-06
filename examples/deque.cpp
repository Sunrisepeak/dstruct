#include <iostream>

#include "dstruct.hpp"


int main() {

    std::cout << "\n\nTesting: " << __FILE__ << std::endl;

    dstruct::DoubleEndedQueue<int, 10> _deque; // 0. test cntor
    dstruct::Deque<int /* Array Size Default 32 */> __deque;
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

//  iterator test(only read/const)
    std::cout << "deque: ";

    for (auto &val : deque) {
        // val = 1; // only-read error
        std::cout << " " << val;
    }
    std::cout << " | ";
    for (auto it = deque.begin(); it != deque.end(); it++) {
        // *it = 1; // only-read error
        std::cout << " " << *it;
    }

    std::cout << std::endl;

// random access (only-read)
    for (int i = 0; i < deque.size(); i++) {
        // deque[i] = 0; // only-read error
        std::cout << "deque[" << i << "] = " << deque[i] << " / ";
        std::cout << "deque[" << -(i + 1) << "] = " << deque[-(i + 1)] << std::endl;
    }

    while (!deque.empty()) {
        deque.pop_front();
        std::cout << "pop front: " << deque.size() << " " << deque.capacity() << std::endl;
    }

    return 0;
}