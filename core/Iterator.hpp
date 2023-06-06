#ifndef __ITERATOR_HPP__DSTRUCT
#define __ITERATOR_HPP__DSTRUCT

namespace dstruct {

struct ForwardIterator { };
struct BidirectionalIterator { };
struct RandomIterator { };

template<typename T, typename Category = ForwardIterator>
class DStructIteratorTypeSpec {
public: // common type
    using CategoryType    = Category;
    using ValueType       = T;
    using PointerType     = ValueType*;
    using ReferenceType   = ValueType&;
    using DifferenceType  = size_t;

protected:
    //template<typename _Iterator>
    //friend DifferenceType distance(const _Iterator &, const _Iterator &);

public: // base op
    ReferenceType operator*() const { return *mPointer; };
    PointerType operator->() const { return mPointer; };
    virtual bool operator!=(const DStructIteratorTypeSpec &it) const { return mPointer != it.mPointer; }
    virtual bool operator==(const DStructIteratorTypeSpec &it) const { return mPointer == it.mPointer; }

protected: // member var
    PointerType mPointer;

/* pls: according to your dstruct impl them
public: // ForwardIterator
    virtual Self& operator++() = 0;
    virtual Self operator++(int) = 0;
public: // BidirectionalIterator
    virtual Self& operator--() = 0;
    virtual Self operator--(int) = 0;
public: // RandomIterator
    virtual Self operator+(int) const = 0;
    virtual Self operator-(int) const = 0;
    virtual Self operator+=(int) const = 0;
    virtual Self operator-=(int) const = 0;
//    virtual ReferenceType operator[](int index) = 0;
//    virtual ValueType operator[](int index) const = 0;
public: // other
    virtual bool operator<(int) const = 0;
    friend DifferenceType operator-(const Self&, const Self&); // outside for your it
*/
};

template<typename RandomIteratorType>
auto distance(const RandomIteratorType &first, const RandomIteratorType &last) -> typename RandomIteratorType::DifferenceType {
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
typename PrimitiveIterator<T>::DifferenceType
operator-(const PrimitiveIterator<T>&, const PrimitiveIterator<T>&);


//  a wrraper/demo for primitive pointer
template <typename T>
class PrimitiveIterator : public DStructIteratorTypeSpec<T, RandomIterator> {
private:
    using __Self = PrimitiveIterator;
    /* 1. */ // friend typename PrimitiveIterator<T>::DifferenceType operator-(const PrimitiveIterator<T>&, const PrimitiveIterator<T>&); // compiler err?
    /* 2. */ //template <typename _T> friend typename PrimitiveIterator<_T>::DifferenceType operator-(const PrimitiveIterator<_T>&, const PrimitiveIterator<_T>&); // ok, but range issue
    friend typename __Self::DifferenceType
    operator-<T>(const __Self&, const __Self&); // explicity template-instance for T

public:
    PrimitiveIterator() = default;
    PrimitiveIterator(T *ptr) : PrimitiveIterator() {
        __Self::mPointer = ptr;
    }

public: // ForwardIterator
    __Self& operator++() { __Self::mPointer++; return *this; };
    __Self operator++(int) { return __Self::mPointer++; };
public: // BidirectionalIterator
    __Self& operator--() { __Self::mPointer--; return *this; };
    __Self operator--(int) { return __Self::mPointer--; };
public: // RandomIterator
    __Self operator+(const int &n) const { return __Self::mPointer + n; };
    __Self operator-(const int &n) const { return __Self::mPointer -n; };
//    typename __Self::ReferenceType operator[](int index) { return __Self::mPointer[index]; }
//    typename __Self::ValueType operator[](int index) const { return __Self::mPointer[index]; };
};


template <typename T>
typename PrimitiveIterator<T>::DifferenceType
operator-(const PrimitiveIterator<T> &last, const PrimitiveIterator<T> &first) {
    return last.mPointer - first.mPointer;
};

// instance (int) function template
// template typename PrimitiveIterator<int>::DifferenceType operator-(const PrimitiveIterator<int>&, const PrimitiveIterator<int>&);

}

#endif