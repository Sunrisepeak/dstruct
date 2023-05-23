#ifndef __COMMON_HPP__DSTRUCT
#define __COMMON_HPP__DSTRUCT

// Don't modify this file easily

#include <dstruct-port.h>
#include <Iterator.hpp>

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

/*
public: // base op
    // status
    virtual SizeType size() const = 0;
    virtual bool empty() const = 0;

    // push/pop
    virtual void push(const T&) = 0;
    virtual ValueType pop() = 0;

    // iterator/range-for support
    virtual IteratorType begin() = 0;
    virtual ConstIteratorType begin() const = 0;
    virtual IteratorType end() = 0;
    virtual ConstIteratorType end() const = 0;
*/
};

template<typename T>
T* construct(void *addr, const T& obj) {
    return new(addr) T(obj); // use T's constructor(copy/spec)
}

template<typename T>
void destory(T *ptr) {
    ptr->~T();
}

};

#endif