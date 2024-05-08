// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef VECTOR_HPP__DSTRUCT
#define VECTOR_HPP__DSTRUCT

#include <core/common.hpp>

namespace dstruct {

template <typename T, typename __Alloc = dstruct::Alloc>
class Vector : public _DStructTypeSpec<T, __Alloc, PrimitiveIterator> {

    DSTRUCT_TYPE_SPEC_HELPER(Vector);

public: // big five
    Vector() :  mSize_d { 0 }, mCapacity_d { 0 }, mC_d { nullptr } { }

    Vector(size_t n, ConstReferenceType element) : Vector() {
        DSTRUCT_ASSERT(n != 0);
        resize(n);
        for (int i = 0; i < n; i++)
             dstruct::construct(mC_d + i, element);
        mSize_d = n;
        mCapacity_d = n;
    }

    DSTRUCT_COPY_SEMANTICS(Vector) {
        clear();
        resize(ds.mCapacity_d);
        mSize_d = ds.mSize_d;
        for (int i = 0; i < mSize_d; i++) {
            dstruct::destroy(mC_d + i);
            dstruct::construct(mC_d + i, ds.mC_d[i]);
        }
        return *this;
    }

    DSTRUCT_MOVE_SEMANTICS(Vector) {
        clear();

        this->mC_d = ds.mC_d;
        this->mSize_d = ds.mSize_d;
        this->mCapacity_d = ds.mCapacity_d;

        // init ds
        ds.mC_d = nullptr;
        ds.mSize_d = ds.mCapacity_d = 0;

        return *this;
    }

    ~Vector() {
        clear();
    }

public: // Capacity
    bool empty() const {
        return mSize_d == 0;
    }

    SizeType size() const {
        return mSize_d;
    }

    SizeType capacity() const {
        return mCapacity_d;
    }

public: // Access
    ConstReferenceType back() const {
        DSTRUCT_ASSERT(mSize_d > 0);
        return mC_d[mSize_d - 1];
    }

    ConstReferenceType front() const {
        return mC_d[0];
    }

    ConstReferenceType operator[](int index) const {
        if (index < 0)
            index = mSize_d + index;
        DSTRUCT_ASSERT(index < static_cast<int>(mSize_d));
        return mC_d[index];
    }

public: // Modifiers
    void push(ConstReferenceType element) {
        push_back(element);
    }

    void push_back(ConstReferenceType element) {
        if (mSize_d + 1 > mCapacity_d) {
            if (mSize_d)
                resize(2 * mSize_d);
            else
                resize(2);
        }
        dstruct::construct(mC_d + mSize_d, element);
        mSize_d++;
    }

    void pop() {
        pop_back();
    }

    void pop_back() {
        DSTRUCT_ASSERT(mSize_d > 0);
        --mSize_d;
        dstruct::destroy(mC_d + mSize_d);
        if (mSize_d < mCapacity_d / 3) {
            resize(mCapacity_d / 2);
        }
    }

    ReferenceType operator[](int index) {
        if (index < 0)
            index = mSize_d + index;
        DSTRUCT_ASSERT(index < static_cast<int>(mSize_d));
        return mC_d[index];
    }

    void clear() {
        if (mC_d) {
            // don't need to destroy haven't contructed area
            for (int i = 0; i < mSize_d; i++) {
                dstruct::destroy(mC_d + i);
            }
            Vector::_Alloc::deallocate(mC_d, mCapacity_d);
        }
        mSize_d = mCapacity_d = 0;
        mC_d = nullptr;
    }

public: // iterator
    IteratorType begin() {
        return mC_d;
    }

    ConstIteratorType begin() const {
        return mC_d;
    }

    IteratorType end() {
        return mC_d + mSize_d;
    }

    ConstIteratorType end() const {
        return mC_d + mSize_d;
    }

public:
    void resize(size_t n) {
        PointerType oldC = mC_d;

        if (n == 0)
            mC_d = nullptr;
        else
            mC_d = Vector::_Alloc::allocate(n);

        if (oldC) {
            DSTRUCT_ASSERT(mCapacity_d != 0);
            for (int i = 0; i < mSize_d; i++) {
                if (i < n) // TODO: use move by check type-tag
                    dstruct::construct(mC_d + i, oldC[i]);
                dstruct::destroy(oldC + i);
            }
            Vector::_Alloc::deallocate(oldC, mCapacity_d);
        }

        mCapacity_d = n;

        if (n < mSize_d)
            mSize_d = n;
    }

    void resize(size_t n, ConstReferenceType element) {

        if (n == 0) {
            resize(0);
            return;
        }

        // alloc
        PointerType oldC = mC_d;
        mC_d = Vector::_Alloc::allocate(n);

        // release
        if (oldC) {
            for (int i = 0; i < mSize_d; i++) {
                if (i < n) // TODO: use move by check type-tag
                    dstruct::construct(mC_d + i, oldC[i]);
                dstruct::destroy(oldC + i);
            }
            Vector::_Alloc::deallocate(oldC, mCapacity_d);
        }

        // set
        for (int i = mSize_d; i < n; i++) {
            dstruct::construct(mC_d + i, element);
        }

        mCapacity_d = mSize_d = n;
    }

protected: // data member
    SizeType mSize_d, mCapacity_d;
    PointerType mC_d;
};

};
#endif