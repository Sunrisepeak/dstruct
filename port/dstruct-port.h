#ifndef __DSTRUCT_PORT_HPP__DSTRUCT
#define __DSTRUCT_PORT_HPP__DSTRUCT

namespace dstruct {

#define _ASSERT(x, panic) \
    if (!(x)) { \
        if (panic) \
            *(static_cast<volatile char *>(0)) = 'E'; \
    }

#define ASSERT(x) _ASSERT(x, true)


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

template<typename T>
struct DSAlloc {
    static T *allocate(size_t n = 1);
    static void deallocate(T *p, size_t n = 1);
};

};

// interface impl
#include <libc-dstruct-port.hpp>

#endif /* __DSTRUCT_PORT_HPP__DSTRUCT */