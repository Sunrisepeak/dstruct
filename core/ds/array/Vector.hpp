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

template <typename T, typename __Alloc = port::Alloc>
class Vector : public DStructTypeSpec<T, __Alloc> {

public: // big five
    Vector() :  _mSize { 0 }, _mCapacity { 0 }, _mC { nullptr } { }

    Vector(size_t n, const T &obj) : Vector() {
        DSTRUCT_ASSERT(n != 0);
        resize(n);
        for (int i = 0; i < n; i++)
             dstruct::construct(_mC + i, obj);
        _mSize = n;
        _mCapacity = n;
    }

    Vector(const Vector& vec) : Vector() { *this = vec; }
    Vector & operator=(const Vector &vec) {
        resize(vec._mCapacity);
        _mSize = vec._mSize;
        for (int i = 0; i < _mSize; i++) {
            //_mC[i] = vec._mC[i]; have some clas haven't operator=
            dstruct::construct(_mC + i, vec._mC[i]);
        }
        return *this;
    }

    Vector(Vector &&vec) : Vector() { *this = dstruct::move(vec); }
    Vector & operator=(Vector &&vec) {
        dstruct::destory(this);

        this->_mC = vec._mC;
        this->_mSize = vec._mSize;
        this->_mCapacity = vec._mCapacity;

        // init vec
        vec._mC = nullptr;
        vec._mSize = vec._mCapacity = 0;

        return *this;
    }

    ~Vector() {
        if (_mC) {
            // don't need to destory haven't contructed area
            for (int i = 0; i < _mSize; i++) {
                dstruct::destory(_mC + i);
            }
            Vector::_Alloc::deallocate(_mC, _mCapacity);
        }
        _mSize = _mCapacity = 0;
        _mC = nullptr;
    }

public: // Capacity
    bool empty() const { return _mSize == 0; }
    size_t size() const { return _mSize; }
    size_t capacity() const { return _mCapacity; }

public: // Access
    T back() const {  DSTRUCT_ASSERT(_mSize > 0); return _mC[_mSize - 1]; }
    T front() const { return _mC[0]; }

public: // Modifiers
    void push(const T &obj) { push_back(obj); }
    void push_back(const T &obj) {
        if (_mSize + 1 > _mCapacity) {
            if (_mSize)
                resize(2 * _mSize);
            else
                resize(2);
        }
        dstruct::construct(_mC + _mSize, obj);
        _mSize++;
    }

    void pop() { pop_back(); }
    void pop_back() {
        DSTRUCT_ASSERT(_mSize > 0);
        --_mSize;
        dstruct::destory(_mC + _mSize);
        if (_mSize < _mCapacity / 3) {
            resize(_mCapacity / 2);
        }
    }

    void resize(size_t n) {
        T *oldC = _mC;
        if (n == 0) _mC = nullptr;
        else _mC = Vector::_Alloc::allocate(n);
        for (int i = 0; i < _mSize; i++) {
            if (i < n)
                dstruct::construct(_mC + i, oldC[i]);
            dstruct::destory(oldC + i);
        }
        if (_mCapacity) {
            DSTRUCT_ASSERT(oldC != nullptr);
            Vector::_Alloc::deallocate(oldC, _mCapacity);
        }
        _mCapacity = n;
        if (n < _mSize)
            _mSize = n;
    }

    void resize(size_t n, const T &obj) {
        // release
        for (int i = 0; i < _mSize; i++) {
            dstruct::destory(_mC + i);
        }
        Vector::_Alloc::deallocate(_mC, _mCapacity);
        // alloc
        _mC = Vector::_Alloc::allocate(n);
        _mCapacity = _mSize = n;
        for (int i = 0; i < _mSize; i++) {
            dstruct::construct(_mC + i, obj);
        }
    }

public: // operator
    T & operator[](int index) {
        if (index < 0)
            index = _mSize + index;
        DSTRUCT_ASSERT(index < static_cast<int>(_mSize));
        return _mC[index];
    }

    T operator[](int index) const {
        if (index < 0)
            index = _mSize + index;
        DSTRUCT_ASSERT(index < static_cast<int>(_mSize));
        return _mC[index];
    }

public: // iterator
    typename Vector::IteratorType begin() { return _mC; }
    typename Vector::ConstIteratorType begin() const { return _mC; }
    typename Vector::IteratorType end() { return _mC + _mSize; }
    typename Vector::ConstIteratorType end() const { return _mC + _mSize; }

protected: // data member
    size_t _mSize, _mCapacity;
    T *_mC;
};

};
#endif