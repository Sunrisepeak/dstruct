// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef ARRAY_HPP_DSTRUCT
#define ARRAY_HPP_DSTRUCT

#include <spec/DStructSpec.hpp>
#include <core/Iterator.hpp>
#include <core/utils.hpp>

namespace dstruct {

template <typename T, size_t N>
class Array : public DStructTypeSpec_<T, void, PrimitiveIterator> {

    DSTRUCT_TYPE_SPEC_HELPER(Array)

public: // big Five

    Array() = default;

    Array(ConstReferenceType element) {
        for (int i = 0; i < N; i++) {
            mData[i] = element;
        }
    }

    Array(port::initializer_list<T> &&list) {
        auto it = list.begin();
        int i = 0;

        while (it != list.end() && i < N) {
            mData[i] = dstruct::move(*it);
            it++;
            i++;
        }

        while (i < N) {
            mData[i] = T();
            i++;
        }
    }

public: // Capacity
    bool empty() const noexcept {
        return N == 0;
    }

    SizeType size() const noexcept {
        return N;
    }

    SizeType capacity() const noexcept {
        return N == 0 ? 1 : N;
    }

public: // Access
    ConstReferenceType back() const noexcept {
        return mData[N - 1];
    }

    ReferenceType back() noexcept {
        return mData[N - 1];
    }

    ConstReferenceType front() const noexcept {
        return mData[0];
    }

    ReferenceType front() noexcept {
        return mData[0];
    }

    ConstReferenceType operator[](int index) const noexcept {
        if (index < 0)
            index = N + index;
        return mData[index];
    }

    ReferenceType operator[](int index) noexcept {
        if (index < 0)
            index = N + index;
        return mData[index];
    }

    PointerType data() noexcept {
        return mData;
    }

    ConstPointerType data() const noexcept {
        return mData;
    }

public: // iterator
    IteratorType begin() noexcept { return mData; }
    ConstIteratorType begin() const noexcept { return mData; }
    ConstIteratorType cbegin() const noexcept { return mData; }

    IteratorType end() noexcept { return mData + N; }
    ConstIteratorType end() const noexcept { return mData + N; }
    ConstIteratorType cend() const noexcept { return mData + N; }

public: // Method chaining
    using SortCmpFunc = bool (*)(ConstReferenceType, ConstReferenceType);
    Array & sort(SortCmpFunc cmp = [](ConstReferenceType a, ConstReferenceType b) { return a < b; }) noexcept {
        // tmp-impl
        for (int i = 0; i < N - 1; ++i) {
            for (int j = 0; j < N - 1 - i; ++j) {
                if (!cmp(mData[j], mData[j + 1])) {
                    dstruct::swap(mData[j], mData[j + 1]);
                }
            }
        }
        return *this;
    }

    Array & reverse() {
        for (int i = 0; i < N / 2; i++) {
            dstruct::swap((*this)[i], (*this)[-1 - i]);
        }
        return *this;
    }

    Array & swap(Array &arr) {
        for (int i = 0; i < N; i++) {
            dstruct::swap(mData[i], arr.mData[i]);
        }
        return *this;
    }

protected:
    T mData[N == 0 ? 1 : N];
}; // Array

};



#endif