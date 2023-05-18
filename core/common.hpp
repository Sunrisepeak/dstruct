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

template<typename T, class Alloc = port::Alloc, typename Iterator = T*>
class DStructTypeSpec {
public:
    using ValueType            = T;
    using ReferenceType        = ValueType&;
    using PointerType          = ValueType*;
    using SizeType             = size_t;
    using DifferenceType       = PointerType;
    using IteratorType         = Iterator;
protected:
    using _Alloc          = DSAlloc<T, Alloc>;
};

template<typename T>
class DStructForwardIteratorTypeSpec {

public:
    using CategoryType    = DStructForwardIteratorTypeSpec<void>;
private:
    using __Self          = DStructForwardIteratorTypeSpec;


public: // common type
    using ValueType       = T;
    using PointerType     = ValueType*;
    using ReferenceType   = ValueType&;
    using DifferenceType  = size_t;

public: // base op, this const keyword: use const keyword to def iterator and const_iterator
    virtual __Self& operator++() = 0;
    //virtual __Self operator++(int) = 0;
    virtual __Self& operator++(int) = 0;

    virtual ReferenceType operator*() = 0;             // for iterator
    virtual const ReferenceType operator*() const = 0; // for const iterator
    virtual PointerType operator->() = 0;
    virtual const ValueType* operator->() const = 0;
};

template<typename DStruct>
class DStructBidirectionalIteratorTypeSpec :
    public DStructForwardIteratorTypeSpec<DStruct> {

// overlay
public:
    using CategoryType    = DStructBidirectionalIteratorTypeSpec<void>;
private:
    using __Self          = DStructBidirectionalIteratorTypeSpec;

public: // op
    virtual __Self& operator--() = 0;
    virtual __Self& operator--(int) = 0;
};

template<typename DStruct>
class DStructRandomIteratorTypeSpec :
    public DStructBidirectionalIteratorTypeSpec<DStruct> {

// overlay
public:
    using CategoryType    = DStructRandomIteratorTypeSpec<void>;
private:
    using __Self          = DStructRandomIteratorTypeSpec;

public: // op
    virtual __Self& operator+(const int&) = 0;
    virtual __Self& operator-(const int&) = 0;
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