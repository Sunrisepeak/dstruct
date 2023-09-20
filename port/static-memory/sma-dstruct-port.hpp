// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef __SMA_DSTRUCT_PORT_HPP__DSTRUCT
#define __SMA_DSTRUCT_PORT_HPP__DSTRUCT

#include <memory/StaticMemAllocator.hpp>

namespace dstruct {
namespace port {

using size_t = long unsigned int;
using DefaultSMA = StaticMemAllocator<1024 * 1024>;

static void * allocate(int bytes) {
    auto *memPtr = DefaultSMA::allocate(bytes);
    if (memPtr == nullptr) {
        DefaultSMA::memory_merge();
        SMA_MEM_VERIFY(memPtr = DefaultSMA::allocate(bytes));
    }
    return memPtr;
}

static void deallocate(void *addr, int bytes) {
    __DSTRUCT_CRASH(DefaultSMA::deallocate(addr, bytes) == false);
}

}

}

#endif