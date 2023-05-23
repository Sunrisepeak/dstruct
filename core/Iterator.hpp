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

public: // base op
    ReferenceType operator*() const { return *mPointer; };
    PointerType operator->() const { return mPointer; };
    bool operator!=(const DStructIteratorTypeSpec &it) const { return mPointer != it.mPointer; }

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
    virtual Self operator+(const int&) const = 0;
    virtual Self operator-(const int&) const = 0;
    virtual ReferenceType operator[](int index) = 0;
    virtual ValueType operator[](int index) const = 0;
*/
};

//  a wrraper/demo for primitive pointer
template <typename T>
class PrimitiveIterator : public DStructIteratorTypeSpec<T, RandomIterator> {

private:
    using __Self = PrimitiveIterator;

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
    typename __Self::ReferenceType operator[](int index) { return __Self::mPointer[index]; }
    typename __Self::ValueType operator[](int index) const { return __Self::mPointer[index]; };
};

}

#endif