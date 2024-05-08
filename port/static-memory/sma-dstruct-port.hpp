// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef SMA_DSTRUCTPORT_HPP_DSTRUCT
#define SMA_DSTRUCTPORT_HPP_DSTRUCT

//#define ENABLE_SMA_LOG
//#define ENABLE_SMA_DUMP

#ifdef ENABLE_SMA_LOG
#include <cstdio>
#undef SMA_LOGD
#define SMA_LOGD(...) printf(__VAARGS__); printf("\n")
#endif

#include <memory/StaticMemAllocator.hpp>

namespace dstruct {
namespace port {

using size_t = long unsigned int;
using DefaultSMA = StaticMemAllocator<1024 * 1024>; // 1M

static void * allocate(int bytes) {
    SMA_LOGD("allocate-start: request %d", bytes);
#ifdef ENABLE_SMA_DUMP
    DefaultSMA::dump();
#endif
    auto *memPtr = DefaultSMA::allocate(bytes);
    if (memPtr == nullptr) {
        SMA_LOGD("memory_merge");
        DefaultSMA::memory_merge();
#ifdef ENABLE_SMA_DUMP
        DefaultSMA::dump();
#endif
        SMA_MEM_VERIFY(memPtr = DefaultSMA::allocate(bytes));
    }
#ifdef ENABLE_SMA_DUMP
    DefaultSMA::dump();
#endif
    SMA_LOGD("allocate-end");
    return memPtr;
}

static void deallocate(void *addr, int bytes) {
    SMA_LOGD("deallocate-start: addr %p, size %d", addr, bytes);
#ifdef ENABLE_SMA_DUMP
    DefaultSMA::dump();
#endif
    DSTRUCT_CRASH(DefaultSMA::deallocate(addr, bytes) == false);
#ifdef ENABLE_SMA_DUMP
    DefaultSMA::dump();
#endif
    SMA_LOGD("deallocate-end");
}

}

}

#endif