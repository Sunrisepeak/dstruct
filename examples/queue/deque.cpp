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

    dstruct::DoubleEndedQueue<int, 10> _deque; // 0. test cntor
    dstruct::Deque<int /* Array Size Default 32 */> __deque;
    dstruct::Deque<int, 5> deque;

// test dynamic resize
    for (int i = 0; i < 20; i++) {
        deque.push_back(i);
        DSTRUCT_ASSERT(deque.back() == i);
        //std::cout << "push back:  " << deque.size() << " " << deque.capacity() << std::endl;
        deque.push_front(i);
        DSTRUCT_ASSERT(deque.front() == i);
        //std::cout << "push front: " << deque.size() << " " << deque.capacity() << std::endl;
    }

    for (int i = 0; i < 10; i++) {
        deque.pop_front();
        //std::cout << "pop front: " << deque.size() << " " << deque.capacity() << std::endl;
        deque.pop_back();
        //std::cout << "pop back:  " << deque.size() << " " << deque.capacity() << std::endl;

    }

    DSTRUCT_ASSERT(deque.size() == 20);

//  iterator test(only read/const)

    for (auto &val : deque) {
        // val = 1; // only-read error
        // std::cout << " " << val;
        int tmp = val;
    }

    for (auto it = deque.begin(); it != deque.end(); it++) {
        // *it = 1; // only-read error
        // std::cout << " " << *it;
    }

// random access (only-read)
    for (int i = 0; i < deque.size(); i++) {
        // deque[i] = 0; // only-read error
        DSTRUCT_ASSERT(deque[i] == deque[-(i + 1)]);
        // std::cout << "deque[" << i << "] = " << deque[i] << " / ";
        // std::cout << "deque[" << -(i + 1) << "] = " << deque[-(i + 1)] << std::endl;
    }

    while (!deque.empty()) {
        deque.pop_front();
        // std::cout << "pop front: " << deque.size() << " " << deque.capacity() << std::endl;
    }

    deque.clear(); deque.push(0);
    deque.clear(); deque.push(1);

    DSTRUCT_ASSERT(deque.size() == 1);

    std::cout << "   pass" << std::endl;

    return 0;
}