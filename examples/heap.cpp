#include <iostream>

#include "dstruct.hpp"


int main() {

    std::cout << "\nTesting: " << __FILE__;

    dstruct::MinHeap<int> _minHeap;
    dstruct::PriorityQueue<int> minHeap;
    dstruct::MaxHeap<int, dstruct::port::Alloc> _maxHeap;
    dstruct::Heap<int, dstruct::greater<int>> maxHeap;

    for (int i = 0; i < 10; i++) {
        maxHeap.push(i);
        minHeap.push(i);
        //std::cout << minHeap.top() << "-" << maxHeap.top() << std::endl;
        DSTRUCT_ASSERT(maxHeap.top() == i);
        DSTRUCT_ASSERT(minHeap.top() == 0);
    }

    for (int i = 0; i < 10; i++) {
        int v1 = minHeap.pop();
        int v2 = maxHeap.pop();
        //std::cout << v1 << "-" << v2 << std::endl;
        DSTRUCT_ASSERT(10 - 1 - v1 == v2);
    }

    DSTRUCT_ASSERT(minHeap.empty());
    DSTRUCT_ASSERT(minHeap.empty());

    std::cout << "   pass" << std::endl;

    return 0;
}