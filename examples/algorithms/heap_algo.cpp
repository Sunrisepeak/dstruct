// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#include <iostream>

#include "dstruct.hpp"


bool cmp_func(int a, int b) {
    return a < b;
}

int main() {

    std::cout << "\nTesting: " << __FILE__;
    int data[10] { 3, 2, 1, 5, 4, 8, 7, 9, 0, 6 };

// heap build
    {
        dstruct::MaxHeap<int> maxHeap(data, data + 10);
        int arr[10];
        
        for (int i = 0; i < 10; i++) arr[i] = data[i];

        dstruct::MaxHeap<int>::build(arr, arr + 10);

        for (int i = 0; i < 10; i++) {
            //std::cout << maxHeap[1 + i] << "-" << arr[i] << std::endl;
            DSTRUCT_ASSERT(maxHeap[1 + i] == arr[i]);
        }
    }


// heap sort
    { // MaxHeap / MinHeap
        int arr[10];
        
        for (int i = 0; i < 10; i++) arr[i] = data[i];

        dstruct::MinHeap<int>::sort(arr, arr + 10);

        for (int i = 0; i < 10; i++) {
            DSTRUCT_ASSERT(i == arr[i]);
        }
    }

    { // Heap
        dstruct::Array<int, 10> arr;
        
        for (int i = 0; i < 10; i++) arr[i] = data[i];

        dstruct::Heap<int, dstruct::greater<int>>::sort(arr.begin(), arr.end());

        for (int i = 0; i < 10; i++) {
            //std::cout << i << "-" << arr[i] << std::endl;
            DSTRUCT_ASSERT(9 - i == arr[i]);
        }
    }

    { // self-def cmp

        dstruct::Heap<int, bool (*)(int, int)> heap1(cmp_func);

        auto myCmp = [](int a, int b) { return a + 1 < b; };

        dstruct::Heap<int, decltype(myCmp)> heap2(myCmp);
    }

    std::cout << "   pass" << std::endl;

    return 0;
}