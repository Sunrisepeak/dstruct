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

//#define ENABLE_SMA_LOG

#ifdef ENABLE_SMA_LOG
#include <cstdio>
#undef SMA_LOGD
#define SMA_LOGD(...) printf(__VA_ARGS__); printf("\n")
#endif

#include <memory/StaticMemAllocator.hpp>

namespace dstruct {
namespace port {

using size_t = long unsigned int;
using DefaultSMA = StaticMemAllocator<1024 * 1024>; // 1M

static void * allocate(int bytes) {
    SMA_LOGD("allocate-start: request %d", bytes);
    DefaultSMA::dump();
    auto *memPtr = DefaultSMA::allocate(bytes);
    if (memPtr == nullptr) {
        SMA_LOGD("memory_merge");
        DefaultSMA::memory_merge();
        DefaultSMA::dump();
        SMA_MEM_VERIFY(memPtr = DefaultSMA::allocate(bytes));
    }
    DefaultSMA::dump();
    SMA_LOGD("allocate-end");
    return memPtr;
}

static void deallocate(void *addr, int bytes) {
    SMA_LOGD("deallocate-start: addr %p, size %d", addr, bytes);
    DefaultSMA::dump();
    __DSTRUCT_CRASH(DefaultSMA::deallocate(addr, bytes) == false);
    DefaultSMA::dump();
    SMA_LOGD("deallocate-end");
}

}

}

#endif