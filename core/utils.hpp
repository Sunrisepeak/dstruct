// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef UTILS_HPP__DSTRUCT
#define UTILS_HPP__DSTRUCT

namespace dstruct {

template <typename T>
struct RemoveReference {
    using Type = T;
};

template <typename T>
struct RemoveReference<T &> {
    using Type = T;
};

template <typename T>
struct RemoveConst {
    using Type = T;
};

template <typename T>
struct RemoveConst<const T> {
    using Type = T;
};

template <typename T>
static typename RemoveConst<T>::Type &
_remove_const(T &data) {
    return *(const_cast<typename RemoveConst<T>::Type *>(&data));
}

template <typename T>
struct IsPointer {
    const static bool value = false;
};

template <typename T>
struct IsPointer<T *> {
    const static bool value = true;
};

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
static typename RemoveReference<T>::Type&& move(T&& arg) noexcept {
    return static_cast<typename RemoveReference<T>::Type&&>(arg);
}


template<typename T>
static constexpr T&& forward(typename RemoveReference<T>::Type&& arg) noexcept {
/*
    // Ref GNU-Version
    static_assert(!std::is_lvalue_reference<T>::value,
    "std::forward must not be used to convert an rvalue to an lvalue");
*/
    return static_cast<T&&>(arg);
}

template <typename T>
static void swap(T &a, T &b) {
    T c = dstruct::move(a);
    a = dstruct::move(b);
    b = dstruct::move(c);
}

template <typename T>
static T max(const T &a, const T &b) {
    return a > b ? a : b;
}

template <typename T>
static T abs(const T &a) {
    return a >= 0 ? a : -a;
}

};

#endif