// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef ARRAY_HPP__DSTRUCT
#define ARRAY_HPP__DSTRUCT

#include <spec/DStructSpec.hpp>
#include <core/Iterator.hpp>
#include <core/utils.hpp>

namespace dstruct {

template <typename T, size_t N>
class Array : public _DStructTypeSpec<T, dstruct::Alloc /*unused*/ , PrimitiveIterator> {

public: // big Five

    Array() = default;

    Array(typename Array::ConstReferenceType element) {
        for (int i = 0; i < N; i++) {
            mC_d[i] = element;
        }
    }

    Array(const Array &arr) {  *this = arr; }
    Array & operator=(const Array &arr) {
        for (int i = 0; i < N; i++) {
            mC_d[i] = arr.mC_d[i];
        }
        return *this;
    }

    Array(Array &&arr) { *this = dstruct::move(arr); };
    Array & operator=(Array &&arr) {
        for (int i = 0; i < N; i++) mC_d[i] = dstruct::move(arr.mC_d[i]);
        return *this;
    }

    ~Array() = default; // array: auto-destroy for every element

public: // Capacity
    bool empty() const {
        return N == 0;
    }

    typename Array::SizeType size() const {
        return N;
    }

    typename Array::SizeType capacity() const {
        return N;
    }

public: // Access
    typename Array::ConstReferenceType back() const {
        return mC_d[N - 1];
    }

    typename Array::ConstReferenceType front() const {
        return mC_d[0];
    }

    typename Array::ConstReferenceType operator[](int index) const {
        if (index < 0)
            index = N + index;
        return mC_d[index];
    }

public: // Modifiers
    typename Array::ReferenceType operator[](int index) {
        if (index < 0)
            index = N + index;
        return mC_d[index];
    }

public: // iterator
    typename Array::IteratorType begin() {
        return mC_d;
    }

    typename Array::ConstIteratorType begin() const {
        return mC_d;
    }

    typename Array::IteratorType end() {
        return mC_d + N;
    }

    typename Array::ConstIteratorType end() const {
        return mC_d + N;
    }

protected:
    T mC_d[N == 0 ? 1 : N];
}; // Array

};



#endif /* __ARRAY_HPP__TSL */