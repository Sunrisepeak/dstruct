// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef ITERATOR_HPP__DSTRUCT
#define ITERATOR_HPP__DSTRUCT

#include <spec/DStructSpec.hpp>

namespace dstruct {

template <typename RandomIteratorType>
static auto distance(const RandomIteratorType &first, const RandomIteratorType &last) -> typename RandomIteratorType::DifferenceType {
    return last - first; // operator-(last, first);
}

/**
 * 
 * pre-declaration of template-friend in PrimitiveIterator
 * 
*/
template <typename>
class PrimitiveIterator;

template <typename T>
static typename PrimitiveIterator<T>::DifferenceType
operator-(const PrimitiveIterator<T>&, const PrimitiveIterator<T>&);


//  a wrraper/demo for primitive pointer
template <typename T>
class PrimitiveIterator : public DStructIteratorTypeSpec<T, RandomIterator> {
private:
    using Self = PrimitiveIterator;
    /* 1. */ // friend typename PrimitiveIterator<T>::DifferenceType operator-(const PrimitiveIterator<T>&, const PrimitiveIterator<T>&); // compiler err?
    /* 2. */ //template <typename _T> friend typename PrimitiveIterator<_T>::DifferenceType operator-(const PrimitiveIterator<_T>&, const PrimitiveIterator<_T>&); // ok, but range issue
    friend typename Self::DifferenceType
    operator-<T>(const Self&, const Self&); // explicity template-instance for T

public:
    PrimitiveIterator() = default;
    PrimitiveIterator(T *ptr) : PrimitiveIterator() {
        Self::_mPointer = ptr;
    }

public: // ForwardIterator
    Self& operator++() { Self::_mPointer++; return *this; };
    Self operator++(int) { return Self::_mPointer++; };
public: // BidirectionalIterator
    Self& operator--() { Self::_mPointer--; return *this; };
    Self operator--(int) { return Self::_mPointer--; };
public: // RandomIterator
    Self operator+(const int &n) const { return Self::_mPointer + n; };
    Self operator-(const int &n) const { return Self::_mPointer - n; };
//    typename Self::ReferenceType operator[](int index) { return Self::_mPointer[index]; }
//    typename Self::ValueType operator[](int index) const { return Self::_mPointer[index]; };
};


template <typename T>
static typename PrimitiveIterator<T>::DifferenceType
operator-(const PrimitiveIterator<T> &last, const PrimitiveIterator<T> &first) {
    return last._mPointer - first._mPointer;
};

// instance (int) function template
// template typename PrimitiveIterator<int>::DifferenceType operator-(const PrimitiveIterator<int>&, const PrimitiveIterator<int>&);

}

#endif