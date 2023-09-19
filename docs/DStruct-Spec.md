# DStruct Spec

## DStruct Spec V0.0.1  - Draft - 2023.9

### Iterator Spec

```cpp
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
    using __Self               = DStructIteratorTypeSpec;

public: // bigfive
    __Self();
    __Self(const __Self &);
    __Self & operator=(const __Self&);
    __Self(__Self &&);
    __Self & operator=(__Self &&)；
    ~__Self();

// Interface Spec
public: // base op
    ReferenceType operator*() const;
    PointerType operator->() const;
    bool operator!=(const __Self &it) const;
    bool operator==(const __Self &it) const;
public: // ForwardIterator
    __Self& operator++();
    __Self operator++(int);
public: // BidirectionalIterator
    __Self& operator--();
    __Self operator--(int);
public: // RandomIterator
    __Self operator+(int) const;
    __Self operator-(int) const;
    __Self operator+=(int) const;
    __Self operator-=(int) const;

// Member Var Spec
protected: // member var
    PointerType _mPointer;
};
```


### Data Structures Spec

```cpp
template <typename T, typename Alloc, template <typename> class Iterator>
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
    using IteratorType         = Iterator<ValueType>;
    using ConstIteratorType    = Iterator<const ValueType>;
protected: // alloc type
    using _Alloc               = DSAlloc<T, Alloc>;
private:
    using __Self               = DStructTypeSpec;

public: // bigfive
    __Self();
    __Self(const __Self &);
    __Self & operator=(const __Self&);
    __Self(__Self &&);
    __Self & operator=(__Self &&)；
    ~__Self();

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
    IteratorType erase(ConstIteratorType pos);
    IteratorType insert(ConstIteratorType pos, ConstReferenceType element);
    Reference operator[](int);
public: // iterator/range-for support
    IteratorType begin();
    ConstIteratorType begin() const;
    IteratorType end();
    ConstIteratorType end() const;
};
```


### Algorithms Spec


### Port & Allocator Spec

```cpp
struct Alloc {
    static void * allocate(size_t bytes);
    static void deallocate(void *addr, size_t bytes);
};
```

