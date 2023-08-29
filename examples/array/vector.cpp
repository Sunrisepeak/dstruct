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

int main() {

    std::cout << "\nTesting: " << __FILE__;

    dstruct::Vector<int> vec(10, 2);

    DSTRUCT_ASSERT(vec.size() == 10);

    for (auto &v : vec) {     // 2. range-for
        //std::cout << v << " " << vec.size() << " " << vec.capacity() << std::endl;
        v = 1;
    }

    DSTRUCT_ASSERT(vec.back() == 1);

    decltype(vec)::ValueType val = 11;

    vec[0] = vec[5] = val;

    DSTRUCT_ASSERT(vec[0] == 11);

    while (!vec.empty()) {
        //std::cout << vec.back() << " " << vec.size() << " " << vec.capacity() << std::endl;
        vec.pop_back();
    }

    DSTRUCT_ASSERT(vec.size() == 0);

    for (int i = 0; i < 10; i++) {
        //std::cout << vec.back() << " " << vec.size() << " " << vec.capacity() << std::endl;
        DSTRUCT_ASSERT(vec.size() == i);
        vec.push_back(i);
        DSTRUCT_ASSERT(vec.back() == i);
    }

    DSTRUCT_ASSERT(vec.size() == 10);

    std::cout << "   pass" << std::endl;

    return 0;
}