// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef __LIBC_DSTRUCT_PORT_HPP__DSTRUCT
#define __LIBC_DSTRUCT_PORT_HPP__DSTRUCT

#include <cstdio>
#include <memory/StaticMemAllocator.hpp>

namespace dstruct {
namespace port {

using size_t = long unsigned int;
using DefaultSMA = StaticMemAllocator<1024 * 1024>;

static void * allocate(int bytes) {
    printf("dstruct::port::allocate1: total-mem: %d, max-block-size: %d, request-alloc %d\n",
        DefaultSMA::free_mem_size(), DefaultSMA::max_free_mblock_size(), bytes
    );
    auto *memPtr = DefaultSMA::allocate(bytes);
    if (memPtr == nullptr) {
        DefaultSMA::memory_merge();
        SMA_MEM_VERIFY(memPtr = DefaultSMA::allocate(bytes));
    }
    printf("dstruct::port::allocate2: total-mem: %d, max-block-size: %d, mem-ptr: %p\n",
        DefaultSMA::free_mem_size(), DefaultSMA::max_free_mblock_size(), memPtr
    );
    return memPtr;
}

static void deallocate(void *addr, int bytes) {
    printf("dstruct::port::deallocate1: total-mem: %d, max-block-size: %d, addr: %p, request-dealloc %d\n",
        DefaultSMA::free_mem_size(), DefaultSMA::max_free_mblock_size(), addr, bytes
    );
    __DSTRUCT_CRASH(DefaultSMA::deallocate(addr, bytes) == false);
    printf("dstruct::port::deallocate2: total-mem: %d, max-block-size: %d, addr: %p, request-dealloc %d\n",
        DefaultSMA::free_mem_size(), DefaultSMA::max_free_mblock_size(), addr, bytes
    );
}

}

}

#endif