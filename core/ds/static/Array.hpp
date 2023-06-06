#ifndef __ARRAY_HPP__DSTRUCT
#define __ARRAY_HPP__DSTRUCT

#include <common.hpp>

namespace dstruct {

template <typename T, size_t N>
class Array : public DStructTypeSpec<T>{

public:

    T & operator[](int index) {
        if (index < 0)
            index = N + index;
        DSTRUCT_ASSERT(index < static_cast<int>(N));
        return _mC[index];
    }

    T operator[](int index) const {
        if (index < 0)
            index = N + index;
        DSTRUCT_ASSERT(index < static_cast<int>(N));
        return _mC[index];
    }

public: // big Five

    Array() /*: _mIterator { _mC }, _mConstIterator { _mC }*/ { };

    Array(const T &value) : Array() {
        for (size_t i = 0; i < N; i++) {
            _mC[i] = value;
        }
    }

    Array(const Array &arr) {
        *this = arr; // by Array & operator=(const Array &arr)
    }

    Array & operator=(const Array &arr) {
        for (size_t i = 0; i < N; i++) {
            _mC[i] = arr._mC[i];
        }
        //_mIterator = arr._mIterator;
        //_mConstIterator = arr._mConstIterator;
        return *this;
    }

    Array(const Array &&) = delete;
    Array & operator=(const Array &&) = delete;

    ~Array() {
        for (int i = 0; i < N; i++) {
            destory(_mC + i);
        }
    }

public: // base op
    size_t size() const {
        return N;
    }

    typename Array::IteratorType begin() {
        return _mC;
    }

    typename Array::ConstIteratorType begin() const {
        return _mC;
    }

    typename Array::ConstIteratorType cbegin() const {
        return _mC;
    }

    typename Array::IteratorType end() {
        return _mC + N;
    }

    typename Array::ConstIteratorType end() const {
        return _mC + N;
    }

    typename Array::ConstIteratorType cend() const {
        return _mC + N;
    }

protected:

    T _mC[N + 1];
    //typename Array::IteratorType _mIterator;
    //typename Array::ConstIteratorType _mConstIterator;
    //T c[N == 0 ? 1 : N];

}; // Array

};



#endif /* __ARRAY_HPP__TSL */