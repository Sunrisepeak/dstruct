#ifndef __LIBC_DSTRUCT_PORT_HPP__DSTRUCT
#define __LIBC_DSTRUCT_PORT_HPP__DSTRUCT

#include <cstdlib>
#include <cassert>

#include <dstruct-port.h>

#define __DSTRUCT_PORT_ASSERT(expr) assert(expr)

namespace dstruct {

void* port::Alloc::allocate(size_t bytes) {
    return malloc(bytes);
}

void port::Alloc::deallocate(void *addr, size_t bytes) {
    free(addr);
}

};

#endif