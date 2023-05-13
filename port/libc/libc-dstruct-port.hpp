#ifndef __LIBC_DSTRUCT_PORT_HPP__DSTRUCT
#define __LIBC_DSTRUCT_PORT_HPP__DSTRUCT

#include <cstdlib>

#include <dstruct-port.h>

namespace dstruct {

template<typename T>
T* DSAlloc<T>::allocate(size_t n) {
    return static_cast<T *>(malloc(n * sizeof(T)));
}

template<typename T>
void DSAlloc<T>::deallocate(T *addr, size_t) {
    free(addr);
}

};

#endif