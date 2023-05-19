#ifndef __COMMON_HPP__DSTRUCT
#define __COMMON_HPP__DSTRUCT

#include <dstruct-port.h>
#include <iterator.hpp>

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

template<
    typename T,
    typename Alloc = port::Alloc,
    template<typename> typename Iterator = PrimitiveIterator
>
class DStructTypeSpec {
public:
    using ValueType            = T;
    using ReferenceType        = ValueType&;
    using PointerType          = ValueType*;
    using SizeType             = size_t;
    using DifferenceType       = PointerType;
    using IteratorType         = Iterator<ValueType>;
    using ConstIteratorType    = Iterator<const ValueType>;
protected:
    using _Alloc          = DSAlloc<T, Alloc>;
};

template<typename T>
T* contruct(void *addr, const T& obj) {
    return new(addr) T(obj); // use T's move/assign contructor
}

template<typename T>
void destory(T *ptr) {
    ptr->~T();
}

};

#endif