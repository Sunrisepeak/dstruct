#ifndef __COMMON_HPP__DSTRUCT
#define __COMMON_HPP__DSTRUCT

#include <dstruct-port.h>

namespace dstruct {

template<typename T, typename Alloc>
struct DSAlloc {
    static T *allocate(size_t n = 1) {
        return static_cast<T *>(Alloc::allocate(n * sizeof(T)));
    }

    static void deallocate(T *ptr, size_t n = 1) {
        Alloc::deallocate(ptr, n * sizeof(T));
    }
};

template<typename T, class Alloc = port::Alloc>
class DStructTypeSpec {
public:
    using ValueType       = T;
    using PointerType     = ValueType*;
    using SizeType        = size_t;
    using DifferenceType  = PointerType;
protected:
    using _Alloc          = DSAlloc<T, Alloc>;
};

};

#endif