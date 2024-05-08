// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef DSTRUCT_SPEC_HPP_DSTRUCTV001
#define DSTRUCT_SPEC_HPP_DSTRUCTV001

#define DSTRUCT_CRASH(expr) \
    if (expr) { \
        *(static_cast<volatile char *>(0)) = 'E'; \
    }

namespace dstruct {

//////////////// - Fundamental types

// https://en.cppreference.com/w/cpp/language/types
using int8_t   = char;
using int16_t  = short;
//using int32_t = int;
using int64_t  = long long;

using uint8_t  = unsigned char;
using uint16_t = unsigned short;
//using uint32_t = unsigned int;
using uint64_t = unsigned long long;

using size_t   = unsigned long long;
using ptr_t    = size_t;


//////////////// - Allocator
/*
// AllocSpec: define Alloc's interface spec
struct Alloc {
    static void * allocate(int bytes);
    static void deallocate(void *addr, int bytes);
};
*/
template <typename T, typename Alloc>
struct AllocSpec {
    static T *allocate(int n = 1) {
        return static_cast<T *>(Alloc::allocate(n * sizeof(T)));
    }

    static void deallocate(T *ptr, int n = 1) {
        Alloc::deallocate(ptr, n * sizeof(T));
    }
};


//////////////// - Iterator

struct ForwardIterator { };
struct BidirectionalIterator { };
struct RandomIterator { };

template <typename T, typename Category = ForwardIterator>
class DStructIteratorTypeSpec {
// Type Spec
public: // common type
    using ValueType            = T;
    using ReferenceType        = ValueType &;
    using ConstReferenceType   = const ValueType &;
    using PointerType          = ValueType *;
    using ConstPointerType     = const ValueType *;
    using SizeType             = unsigned long long;
    using DifferenceType       = long long;
protected: // iterator category type
    using CategoryType         = Category;
private:
    using Self               = DStructIteratorTypeSpec;

/*
public: // bigfive
    Self();
    Self(const Self &);
    Self & operator=(const Self&);
    Self(Self &&);
    Self & operator=(Self &&);
    ~Self();
*/

// Interface Spec
public: // base op
    ReferenceType operator*() const { return *mPointer_d; }
    PointerType operator->() const { return mPointer_d; }
    virtual bool operator!=(const DStructIteratorTypeSpec &it) const { return mPointer_d != it.mPointer_d; }
    virtual bool operator==(const DStructIteratorTypeSpec &it) const { return mPointer_d == it.mPointer_d; }

/* pls: according to your dstruct impl them
public: // ForwardIterator
    Self& operator++();
    Self operator++(int);
public: // BidirectionalIterator
    Self& operator--();
    Self operator--(int);
public: // RandomIterator
    Self operator+(int) const;
    Self operator-(int) const;
    Self operator+=(int) const;
    Self operator-=(int) const;
*/

// Member Var Spec
protected: // member var
    PointerType mPointer_d;
};


//////////////// - Data Structures

template <typename T, typename Alloc, typename Iterator, typename ConstIterator>
class DStructTypeSpec {
// Type Spec
public: // common type
    using ValueType            = T;
    using ReferenceType        = ValueType &;
    using ConstReferenceType   = const ValueType &;
    using PointerType          = ValueType *;
    using ConstPointerType     = const ValueType *;
    using SizeType             = unsigned long long;
    using DifferenceType       = long long;
public: // iterator type
    using IteratorType         = Iterator;
    using ConstIteratorType    = ConstIterator;
protected: // alloc type
    using Alloc_               = AllocSpec<T, Alloc>;
private:
    using Self               = DStructTypeSpec;

/* pls: according to your dstruct impl them
public: // bigfive
    Self();
    Self(const Self &);
    Self & operator=(const Self&);
    Self(Self &&);
    Self & operator=(Self &&)；
    ~Self();

// Interface Spec
public: // Capacity
    bool empty() const;
    SizeType size() const;
    SizeType capacity() const;
public: // Access
    ConstReference back() const;
    ConstReference front() const;
    ConstReference operator[](int) const;
public: // Modifiers
    void push(ConstReferenceType element);
        void push_back(ConstReferenceType element);
        void push_front(ConstReferenceType element);
    void pop();
        void pop_back();
        void pop_front();
    IteratorType insert(ConstIteratorType pos, ConstReferenceType element);
    IteratorType erase(ConstIteratorType pos);
    Reference operator[](int);
    void clear();
public: // iterator/range-for support
    IteratorType begin();
    ConstIteratorType begin() const;
    IteratorType end();
    ConstIteratorType end() const;
*/
};

template <typename T, typename Alloc, template <typename> class Iterator>
using DStructTypeSpec_ = DStructTypeSpec<T, Alloc, Iterator<T>, Iterator<const T>>;

};

#endif