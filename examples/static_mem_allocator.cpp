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

#define MEM_1M_BYTE 1024 * 1024

// define SMA, 1M size
using MySMA_1M = dstruct::StaticMemAllocator<MEM_1M_BYTE>;

int main() {

    std::cout << "\nTesting: " << __FILE__;

    dstruct::Vector<int, MySMA_1M> vec;

    //std::cout << MySMA_1M::free_mem_size() << std::endl;
    DSTRUCT_ASSERT(MySMA_1M::free_mem_size() == MEM_1M_BYTE);

    vec.resize(8, 1); // 8 * 4 = 32

    //std::cout << MySMA_1M::free_mem_size() << std::endl;
    DSTRUCT_ASSERT(MySMA_1M::free_mem_size() == MEM_1M_BYTE - vec.capacity() * sizeof(int));

    vec.push_back(4);

    //std::cout << MySMA_1M::free_mem_size() << std::endl;
    DSTRUCT_ASSERT(MySMA_1M::free_mem_size() == MEM_1M_BYTE - MySMA_1M::MEM_ALIGN_ROUND_UP(vec.capacity() * sizeof(int)));

    std::cout << "   pass" << std::endl;

    return 0;
}