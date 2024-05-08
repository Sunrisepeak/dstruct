// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef COMMON_HPP__DSTRUCT
#define COMMON_HPP__DSTRUCT

// Don't modify this file easily
#include <spec/DStructSpec.hpp>
#include <port/dstruct-port.h>
#include <core/utils.hpp>
#include <core/Iterator.hpp>

namespace dstruct {

#define DSTRUCT_TYPE_SPEC_HELPER(DStruct) \
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

#define DSTRUCT_COPY_SEMANTICS(DStruct) \
    DStruct(const DStruct &ds) : DStruct() { *this = ds; } \
    DStruct & operator=(const DStruct &ds) { \
        if (this == &ds) return *this; \
        return _dstruct_copy_sema(ds); \
    } \
    DStruct & _dstruct_copy_sema(const DStruct &ds)

#define DSTRUCT_MOVE_SEMANTICS(DStruct) \
    DStruct(DStruct &&ds) : DStruct() { *this = dstruct::move(ds); } \
    DStruct & operator=(DStruct &&ds) { \
        if (this == &ds) return *this; \
        return _dstruct_move_sema(dstruct::move(ds)); \
    } \
    DStruct & _dstruct_move_sema(DStruct &&ds)

// TODO: Check placement new for primitive type(example pointer) and self-def type
// https://en.cppreference.com/w/cpp/language/new
// https://en.cppreference.com/w/cpp/memory/new/operator_new
template <typename T>
static T* construct(void *addr, const T& obj) noexcept {
    static DStructPlacementNewFlag placementNewFlag;
    DSTRUCT_CRASH(addr == nullptr);
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
    DSTRUCT_CRASH(addr == nullptr);
    *(reinterpret_cast<int *>(addr)) = obj;
    return reinterpret_cast<int *>(addr);
}

template <typename T>
static void destroy(T *addr) noexcept {
/*
    // method1, but have a litte issue
    static DStructPlacementNewFlag placementNewFlag;
    delete(ptr, &placementNewFlag);
*/
    // method2, skip detele
    DSTRUCT_CRASH(addr == nullptr); // addr is nullptr, trigger crash
    addr->~T();
}

};

#endif