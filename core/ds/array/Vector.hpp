// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef __VECTOR_HPP__DSTRUCT
#define __VECTOR_HPP__DSTRUCT

#include <core/common.hpp>

namespace dstruct {

template <typename T, typename __Alloc = dstruct::Alloc>
class Vector : public _DStructTypeSpec<T, __Alloc, PrimitiveIterator> {

    DSTRUCT_TYPE_SPEC_HELPER(Vector);

public: // big five
    Vector() :  _mSize { 0 }, _mCapacity { 0 }, _mC { nullptr } { }

    Vector(size_t n, ConstReferenceType element) : Vector() {
        DSTRUCT_ASSERT(n != 0);
        resize(n);
        for (int i = 0; i < n; i++)
             dstruct::construct(_mC + i, element);
        _mSize = n;
        _mCapacity = n;
    }

    DSTRUCT_COPY_SEMANTICS(Vector) {
        clear();
        resize(ds._mCapacity);
        _mSize = ds._mSize;
        for (int i = 0; i < _mSize; i++) {
            dstruct::destroy(_mC + i);
            dstruct::construct(_mC + i, ds._mC[i]);
        }
        return *this;
    }

    DSTRUCT_MOVE_SEMANTICS(Vector) {
        clear();

        this->_mC = ds._mC;
        this->_mSize = ds._mSize;
        this->_mCapacity = ds._mCapacity;

        // init ds
        ds._mC = nullptr;
        ds._mSize = ds._mCapacity = 0;

        return *this;
    }

    ~Vector() {
        clear();
    }

public: // Capacity
    bool empty() const {
        return _mSize == 0;
    }

    SizeType size() const {
        return _mSize;
    }

    SizeType capacity() const {
        return _mCapacity;
    }

public: // Access
    ConstReferenceType back() const {
        DSTRUCT_ASSERT(_mSize > 0);
        return _mC[_mSize - 1];
    }

    ConstReferenceType front() const {
        return _mC[0];
    }

    ConstReferenceType operator[](int index) const {
        if (index < 0)
            index = _mSize + index;
        DSTRUCT_ASSERT(index < static_cast<int>(_mSize));
        return _mC[index];
    }

public: // Modifiers
    void push(ConstReferenceType element) {
        push_back(element);
    }

    void push_back(ConstReferenceType element) {
        if (_mSize + 1 > _mCapacity) {
            if (_mSize)
                resize(2 * _mSize);
            else
                resize(2);
        }
        dstruct::construct(_mC + _mSize, element);
        _mSize++;
    }

    void pop() {
        pop_back();
    }

    void pop_back() {
        DSTRUCT_ASSERT(_mSize > 0);
        --_mSize;
        dstruct::destroy(_mC + _mSize);
        if (_mSize < _mCapacity / 3) {
            resize(_mCapacity / 2);
        }
    }

    ReferenceType operator[](int index) {
        if (index < 0)
            index = _mSize + index;
        DSTRUCT_ASSERT(index < static_cast<int>(_mSize));
        return _mC[index];
    }

    void clear() {
        if (_mC) {
            // don't need to destroy haven't contructed area
            for (int i = 0; i < _mSize; i++) {
                dstruct::destroy(_mC + i);
            }
            Vector::_Alloc::deallocate(_mC, _mCapacity);
        }
        _mSize = _mCapacity = 0;
        _mC = nullptr;
    }

public: // iterator
    IteratorType begin() {
        return _mC;
    }

    ConstIteratorType begin() const {
        return _mC;
    }

    IteratorType end() {
        return _mC + _mSize;
    }

    ConstIteratorType end() const {
        return _mC + _mSize;
    }

public:
    void resize(size_t n) {
        PointerType oldC = _mC;
        if (n == 0) _mC = nullptr;
        else _mC = Vector::_Alloc::allocate(n);
        for (int i = 0; i < _mSize; i++) {
            if (i < n) // TODO: use move by check type-tag
                dstruct::construct(_mC + i, oldC[i]);
            dstruct::destroy(oldC + i);
        }
        if (_mCapacity) {
            DSTRUCT_ASSERT(oldC != nullptr);
            Vector::_Alloc::deallocate(oldC, _mCapacity);
        }
        _mCapacity = n;
        if (n < _mSize)
            _mSize = n;
    }

    void resize(size_t n, ConstReferenceType element) {
        // alloc
        PointerType oldC = _mC;
        _mC = Vector::_Alloc::allocate(n);

        // release
        for (int i = 0; i < _mSize; i++) {
            if (i < n) // TODO: use move by check type-tag
                dstruct::construct(_mC + i, oldC[i]);
            dstruct::destroy(oldC + i);
        }
        Vector::_Alloc::deallocate(oldC, _mCapacity);

        // set
        _mCapacity = _mSize = n;
        for (int i = n; i < _mSize; i++) {
            dstruct::construct(_mC + i, element);
        }
    }

protected: // data member
    SizeType _mSize, _mCapacity;
    PointerType _mC;
};

};
#endif