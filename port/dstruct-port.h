// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef __DSTRUCT_PORT_HPP__DSTRUCT
#define __DSTRUCT_PORT_HPP__DSTRUCT

namespace dstruct {
namespace port { // pls impl/port the alloc
    static void *allocate(int bytes);
    static void deallocate(void *addr, int bytes);
} // namespace port end

struct Alloc {
    static void * allocate(int bytes) {
        return dstruct::port::allocate(bytes);
    }

    static void deallocate(void *addr, int bytes) {
        dstruct::port::deallocate(addr, bytes);
    }
};

} // namespace dstruct

// interface impl
#ifdef ENABLE_SMA
#include <port/static-memory/sma-dstruct-port.hpp>
#else
#include <port/libc/libc-dstruct-port.hpp>
#endif

#ifdef __DSTRUCT_PORT_ASSERT
#define DSTRUCT_ASSERT(expr) __DSTRUCT_PORT_ASSERT(expr)
#else
#define DSTRUCT_ASSERT(expr) __DSTRUCT_CRASH(!(expr))
#endif

struct DStructPlacementNewFlag { };
inline void * operator new(dstruct::port::size_t sz, void *ptr, DStructPlacementNewFlag *) noexcept { return ptr; }
// void operator delete(void *ptr, DStructPlacementNewFlag *) {  } haven't used

#endif /* __DSTRUCT_PORT_HPP__DSTRUCT */