#ifndef __VECTOR_HPP__DSTRUCT
#define __VECTOR_HPP__DSTRUCT

#include <common.hpp>

namespace dstruct {

template <typename T, typename __Alloc = port::Alloc>
class Vector : public DStructTypeSpec<T, __Alloc> {

public:
    Vector() :  _mSize { 0 }, _mCapacity { 0 }, _mC { nullptr } { }

    Vector(const T &obj, size_t n) : Vector() {
        _resize(n);
        for (int i = 0; i < n; i++)
            _mC[i] = obj;
        _mSize = n;
        _mCapacity = n;
    }

    ~Vector() {
        if (_mC) {
            Vector::_Alloc::deallocate(_mC, _mCapacity);
        }
    }

public:

    T & operator[](int index) {
        if (index < 0)
            index = _mSize + index;
        return _mC[index];
    }

    T operator[](int index) const {
        if (index < 0)
            index = _mSize + index;
        return _mC[index];
    }
public:

    bool empty() const {
        return _mSize == 0;
    }

    size_t size() const {
        return _mSize;
    }

    size_t capacity() const {
        return _mCapacity;
    }

    T back() const {
        return _mC[_mSize - 1];
    }

public: // Modifiers

    void push_back(const T &obj) {
        if (_mSize + 1 > _mCapacity) {
            _resize(2 * _mSize);
        }
        _mC[_mSize++] = obj;
    }

    void pop_back() {
        _mSize--;
        if (_mSize < _mCapacity / 3) {
            _resize(_mCapacity / 2);
        }
    }

public: // other
    typename Vector::IteratorType begin() {
        return _mC;
    }

    const T * begin() const {
        return _mC;
    }

    typename Vector::IteratorType end() {
        return _mC + _mSize;
    }

    const T * end() const {
        return _mC + _mSize;
    }

protected:

    size_t _mSize, _mCapacity;
    T *_mC;

    void _resize(size_t n) {
        T *oldC = _mC;
        _mC = Vector::_Alloc::allocate(n);
        for (int i = 0; i < _mSize; i++) {
            contruct(_mC + i, oldC[i]);
            destory(oldC + i);
        }
        Vector::_Alloc::deallocate(oldC, _mCapacity);
        _mCapacity = n;
    }
};

};
#endif