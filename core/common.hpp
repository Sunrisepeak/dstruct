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
#include <spec/DStructSpec.hpp>
#include <port/dstruct-port.h>
#include <core/types.hpp>
#include <core/Iterator.hpp>

namespace dstruct {

#define DSTRUCT_TYPE_SPEC_DECLARE(DStruct) \
public: \
    using ValueType            = typename DStruct::ValueType; \
    using ReferenceType        = typename DStruct::ReferenceType; \
    using ConstReferenceType   = typename DStruct::ConstReferenceType; \
    using PointerType          = typename DStruct::PointerType; \
    using ConstPointerType     = typename DStruct::ConstPointerType; \
    using SizeType             = typename DStruct::SizeType; \
    using DifferenceType       = typename DStruct::DifferenceType; \
public: \ 
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
static T* construct(void *addr, const T& obj) noexcept {
    static DStructPlacementNewFlag placementNewFlag;
    __DSTRUCT_CRASH(addr == nullptr);
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
int * construct<int>(void *addr, const int& obj) noexcept {
    __DSTRUCT_CRASH(addr == nullptr);
    *(reinterpret_cast<int *>(addr)) = obj;
    return reinterpret_cast<int *>(addr);
}

template <typename T>
static void destory(T *addr) noexcept {
/*
    // method1, but have a litte issue
    static DStructPlacementNewFlag placementNewFlag;
    delete(ptr, &placementNewFlag);
*/
    // method2, skip detele
    __DSTRUCT_CRASH(addr == nullptr); // addr is nullptr, trigger crash
    addr->~T();
}

};

#endif