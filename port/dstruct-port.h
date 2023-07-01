#ifndef __DSTRUCT_PORT_HPP__DSTRUCT
#define __DSTRUCT_PORT_HPP__DSTRUCT

namespace dstruct {

using int8_t = char;
using int16_t = short;
using int32_t = int;
using int64_t = long long;

using uint8_t = unsigned char;
using uint16_t = unsigned short;
using uint32_t = unsigned int;
using uint64_t = unsigned long long;

using size_t = unsigned long long;
using ptr_t = size_t;


#define __DSTRUCT_CRASH(expr) \
    if (!(expr)) { \
        *(static_cast<volatile char *>(0)) = 'E'; \
    }

namespace port {
// pls impl/port the alloc
/* 
struct Alloc
{
    static void *allocate(size_t bytes);
    static void deallocate(void *addr, size_t bytes);
};
*/
}

}

// interface impl
#include <port/libc/libc-dstruct-port.hpp>

#ifdef __DSTRUCT_PORT_ASSERT
#define DSTRUCT_ASSERT(expr) __DSTRUCT_PORT_ASSERT(expr)
#else
#define DSTRUCT_ASSERT(expr) __DSTRUCT_CRASH(expr)
#endif

#endif /* __DSTRUCT_PORT_HPP__DSTRUCT */