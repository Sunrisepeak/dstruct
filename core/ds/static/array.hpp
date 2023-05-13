#ifndef __ARRAY_HPP__DSTRUCT
#define __ARRAY_HPP__DSTRUCT

#include <dstruct-port.h>

namespace dstruct {

template <typename T, size_t N>
class Array {

public:

    T & operator[](int index) {
        if (index < 0)
            index = N + index;
        ASSERT(index < static_cast<int>(N));
        return __mC[index];
    }

    T operator[](int index) const {
        if (index < 0)
            index = N + index;
        ASSERT(index < static_cast<int>(N));
        return __mC[index];
    }

public: // big Five

    Array() = default;

    Array(const T &value) {
        for (size_t i = 0; i < N; i++) {
            __mC[i] = value;
        }
    }

    Array(const Array &arr) {
        *this = arr; // by Array & operator=(const Array &arr)
    }

    Array & operator=(const Array &arr) {
        for (size_t i = 0; i < N; i++) {
            __mC[i] = arr.__mC[i];
        }
        return *this;
    }

    Array(const Array &&) = delete;
    Array & operator=(const Array &&) = delete;

    size_t size() const {
        return N;
    }

    T * begin() {
        return __mC;
    }

    const T * begin() const {
        return __mC;
    }

    T * end() {
        return __mC + N;
    }

    const T * end() const {
        return __mC + N;
    }

private:

    T __mC[N + 1];
    //T c[N == 0 ? 1 : N];

}; // Array

};



#endif /* __ARRAY_HPP__TSL */