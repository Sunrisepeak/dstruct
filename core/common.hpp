// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef __COMMON_HPP__DSTRUCT
#define __COMMON_HPP__DSTRUCT

// Don't modify this file easily

#include <port/dstruct-port.h>
#include <core/types.hpp>
#include <core/Iterator.hpp>

namespace dstruct {

template <typename T, typename Alloc>
struct DSAlloc {
    static T *allocate(size_t n = 1) {
        return static_cast<T *>(Alloc::allocate(n * sizeof(T)));
    }

    static void deallocate(T *ptr, size_t n = 1) {
        Alloc::deallocate(ptr, n * sizeof(T));
    }
};

template <
    typename T,
    typename Alloc = port::Alloc,
    template <typename> class Iterator = PrimitiveIterator
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
    using _Alloc               = DSAlloc<T, Alloc>;

/*
public: // Capacity
    virtual SizeType size() const = 0;
    virtual SizeType capacity() const = 0;
    virtual bool empty() const = 0;

public: // Access
    // check
    virtual ValueType back() const = 0;
    virtual ValueType front() const = 0;

public: // Modifiers
    virtual void push(const T&) = 0;
    virtual void push_back() = 0;
    virtual void push_front() = 0;
    virtual void pop() = 0;
    virtual void pop_back() = 0;
    virtual void pop_front() = 0;

public: // operator
    virtual const ValueType & operator[](int) = 0;
    virtual ValueType operator[](int) const = 0;

public: // iterator/range-for support
    virtual IteratorType begin() = 0;
    virtual ConstIteratorType begin() const = 0;
    virtual IteratorType end() = 0;
    virtual ConstIteratorType end() const = 0;
*/
};

template <
    typename T,
    typename Iterator = PrimitiveIterator<T>,
    typename ConstIterator = PrimitiveIterator<const T>,
    typename Alloc = port::Alloc
>
class _DStructTypeSpec {
public:
    using ValueType            = T;
    using ReferenceType        = ValueType&;
    using PointerType          = ValueType*;
    using SizeType             = size_t;
    using DifferenceType       = PointerType;
    using IteratorType         = Iterator;
    using ConstIteratorType    = ConstIterator;
protected:
    using _Alloc          = DSAlloc<T, Alloc>;
};

#define DSTRUCT_TYPE_SPEC_DECLARE(DStruct) \
public: \
    using ValueType            = typename DStruct::ValueType; \
    using ReferenceType        = typename DStruct::ReferenceType; \
    using PointerType          = typename DStruct::PointerType; \
    using SizeType             = typename DStruct::SizeType; \
    using DifferenceType       = typename DStruct::DifferenceType; \
    using IteratorType         = typename DStruct::IteratorType; \
    using ConstIteratorType    = typename DStruct::ConstIteratorType;

template <typename T>
static typename types::RemoveReference<T>::Type&& move(T&& arg) {
    return static_cast<typename types::RemoveReference<T>::Type&&>(arg);
}


template <typename T>
struct less {
    bool operator()(const T& a, const T& b) const {
        return a < b;
    }
};

template <typename T>
struct less<T *> {
    bool operator()(T *a, T *b) const {
        if (b == nullptr) return false;
        if (a == nullptr) return true;
        return a < b;
    }
};

template <typename T>
struct greater {
    bool operator()(const T& a, const T& b) const {
        return a > b;
    }
};

template <typename T>
struct greater<T *> {
    bool operator()(const T *a, const T *b) const {
        if (a == nullptr) return false;
        if (b == nullptr) return true;
        return a > b;
    }
};

template <typename T>
static void swap(T &a, T &b) {
    T c = dstruct::move(a);
    a = dstruct::move(b);
    b = dstruct::move(c);
}

// TODO: Check placement new for primitive type(example pointer) and self-def type
// https://en.cppreference.com/w/cpp/language/new
// https://en.cppreference.com/w/cpp/memory/new/operator_new
template <typename T>
static T* construct(void *addr, const T& obj) {
    static DStructPlacementNewFlag placementNewFlag;
    return new(addr, &placementNewFlag) T(obj); // use T's constructor(copy/spec)
}

/*
// partial specialization only for type, func template pls use overload
template <typename T>
static T ** construct(void *addr, const T *obj) {
    T **ptr = static_cast<T **>(addr);
    *ptr = obj;
    return ptr;
}
*/

template <>
int * construct<int>(void *addr, const int& obj) {
    *(reinterpret_cast<int *>(addr)) = obj;
    return reinterpret_cast<int *>(addr);
}

template <typename T>
static void destory(T *ptr) {
/*
    // method1, but have a litte issue
    static DStructPlacementNewFlag placementNewFlag;
    delete(ptr, &placementNewFlag);
*/
    // method2, skip detele
    if (ptr)
        ptr->~T();
}

};

#endif