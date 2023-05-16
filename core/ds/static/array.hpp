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

    Array() = default;

    Array(const T &value) {
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
        return *this;
    }

    Array(const Array &&) = delete;
    Array & operator=(const Array &&) = delete;

    size_t size() const {
        return N;
    }

    T * begin() {
        return _mC;
    }

    const T * begin() const {
        return _mC;
    }

    T * end() {
        return _mC + N;
    }

    const T * end() const {
        return _mC + N;
    }

protected:

    T _mC[N + 1];
    //T c[N == 0 ? 1 : N];

}; // Array

};



#endif /* __ARRAY_HPP__TSL */