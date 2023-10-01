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

    dstruct::String s = "Hello";

    //std::cout << s.c_str() << std::endl;
    DSTRUCT_ASSERT(s == "Hello");

    s += ", DStruct!";

    for (auto c : s) {
        //std::cout << c << std::endl;
    }

    //std::cout << s.c_str() << std::endl;
    DSTRUCT_ASSERT(s == "Hello, DStruct!");

    std::cout << "   pass" << std::endl;

    return 0;
}