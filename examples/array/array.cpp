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

    dstruct::Array<int, 10> arr(2); // 0. test cntor

    decltype(arr)::ValueType val = 6;

    arr[0] = arr[-1] = val;       // 1. test subscript access & assignment

    const auto backup = arr;
    for (auto &v : backup) {     // 2. test const / range-for
        //std::cout << v << " ";
        // v = 1; // error
    }

    //std::cout << std::endl << arr[-1] << std::endl;
    DSTRUCT_ASSERT(arr[-1] == val);

    for (int i = 0; i < arr.size(); i++) {
        arr[i] = i;
    }

    for (int i = 0; i < arr.size(); i++) { // 3.  positive / negative subscript access
        //std::cout << backup[-(i + 1)] << " : " << arr[i] << std::endl;
        DSTRUCT_ASSERT(arr[i] == arr[-(arr.size()) + i]);
    }

    std::cout << "   pass" << std::endl;

    return 0;
}