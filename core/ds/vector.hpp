#ifndef __VECTOR_HPP__DSTRUCT
#define __VECTOR_HPP__DSTRUCT

#include <dstruct-port.h>

namespace dstruct {

template <typename T, typename Alloc = DSAlloc<T>>
class Vector {

public:
    Vector() :  __mSize { 0 }, __mCapacity { 0 }, __mC { nullptr } { }

    Vector(const T &obj, size_t n) : Vector() {
        __resize(n);
        for (int i = 0; i < n; i++)
            __mC[i] = obj;
        __mSize = n;
        __mCapacity = n;
    }

    ~Vector() {
        if (__mC) {
            Alloc::deallocate(__mC, __mCapacity);
        }
    }

public:

    T & operator[](int index) {
        if (index < 0)
            index = __mSize + index;
        return __mC[index];
    }

    T operator[](int index) const {
        if (index < 0)
            index = __mSize + index;
        return __mC[index];
    }
public:

    bool empty() const {
        return __mSize == 0;
    }

    size_t size() const {
        return __mSize;
    }

    size_t capacity() const {
        return __mCapacity;
    }

    T back() const {
        return __mC[__mSize - 1];
    }

public: // Modifiers

    void push_back(const T &obj) {
        if (__mSize + 1 > __mCapacity) {
            __resize(2 * __mSize);
        }
        __mC[__mSize++] = obj;
    }

    void pop_back() {
        __mSize--;
        if (__mSize < __mCapacity / 3) {
            __resize(__mCapacity / 2);
        }
    }

public: // other
    T * begin() {
        return __mC;
    }

    const T * begin() const {
        return __mC;
    }

    T * end() {
        return __mC + __mSize;
    }

    const T * end() const {
        return __mC + __mSize;
    }

private:

    size_t __mSize, __mCapacity;
    T *__mC;

    void __resize(size_t n) {
        T *oldC = __mC;
        __mC = Alloc::allocate(n);
        for (int i = 0; i < __mSize; i++) {
            __mC[i] = oldC[i];
        }
        Alloc::deallocate(oldC, __mCapacity);
        __mCapacity = n;
    }
};

};
#endif