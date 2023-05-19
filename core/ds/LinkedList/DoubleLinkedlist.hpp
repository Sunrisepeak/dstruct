#ifndef __DOUBLE_LINKED_LIST_HPP__DSTRUCT
#define __DOUBLE_LINKED_LIST_HPP__DSTRUCT

#include <common.hpp>

namespace dstruct {

template<typename T>
struct _LNode {
    _LNode *next, *prev;
    T data;

    _LNode(const T &val = 0) : next { nullptr }, prev { nullptr }, data { val } { }
};

template<typename T>
class _DoubleLinkedListIterator : public DStructIteratorTypeSpec<T, BidirectionalIterator> {
protected:
    using _Node  = _LNode<T>;
private:
    using __Self = _DoubleLinkedListIterator;

public: // big five
    _DoubleLinkedListIterator(_Node *ptr) {
        __sync(ptr);
    }

public: // ForwardIterator
    __Self& operator++() { 
        __sync(_mLNodePtr->next);
        return *this;
    };
    __Self operator++(int) {
        _Node *oldPtr = _mLNodePtr;
        __sync(_mLNodePtr->next);
        return oldPtr;
    };
public: // BidirectionalIterator
    __Self& operator--() {
        __sync(_mLNodePtr->prev);
        return *this;
    };
    __Self operator--(int) {
        _Node *oldPtr = _mLNodePtr;
        __sync(_mLNodePtr->prev);
        return oldPtr;
    };
private:
    // update _mLNodePtr and mPointer
    void __sync(_Node *ptr) {
        _mLNodePtr = ptr;
        __Self::mPointer = &(ptr->data);
    }
protected:
    _Node *_mLNodePtr;
};

template<typename T, typename Alloc = port::Alloc>
class DoubleLinkedList : public DStructTypeSpec<T, Alloc, _DoubleLinkedListIterator> {

protected:
    using _Node      = _LNode<T>; 
    using _AllocNode = DSAlloc<_Node, Alloc>;


public: // big five
    DoubleLinkedList() {
        _mHeadNode.next = _mHeadNode.prev = &_mHeadNode;
        _mSize = 0;
    }

    DoubleLinkedList(const T &obj, size_t n) : DoubleLinkedList() {
        for (int i = 0; i < n; i++) {
            push_back(obj);
        }
    }

    ~DoubleLinkedList() {
        while (!empty()) {
            pop_back();
        }
    }

public: // base op
    
    bool empty() const {
        // Note: when _mSize == 1 ->  _mHeadNode.next == _mHeadNode.prev
        if (_mSize == 0) {
            DSTRUCT_ASSERT(_mHeadNode.next == _mHeadNode.prev);
            return true;
        }
        return false;
    }

    typename DoubleLinkedList::SizeType size() const {
        return _mSize;
    }

    T back() const {
        DSTRUCT_ASSERT(_mSize > 0);
        return _mHeadNode.prev->data;
    }

    void push_back(const T &obj) {
        // 1. alloc and construct node
        _Node *nPtr = _AllocNode::allocate();
        construct(nPtr, _Node(obj));
        // 2. nPtr point to list
        nPtr->next = &_mHeadNode;
        nPtr->prev = _mHeadNode.prev;
        // 3. list point to new node
        _mHeadNode.prev->next = nPtr;
        _mHeadNode.prev = nPtr;
        // 4. increase size
        _mSize++;
    }

    T pop_back() {
        DSTRUCT_ASSERT(_mSize > 0);

        // 1. get node ptr
        _Node *nPtr = _mHeadNode.prev;
        // 2. del node from list
        nPtr->prev->next = nPtr->next;
        //nPtr->next->prev = nPtr->prev;
        _mHeadNode.prev = nPtr->prev;
        // 3. get data
        typename DoubleLinkedList::ValueType data = nPtr->data;
        // 4. free and decrease size/len
        destory(nPtr);
        _AllocNode::deallocate(nPtr);
        _mSize--;

        return data;
    }

public: // support it/range-for

    typename DoubleLinkedList::IteratorType
    begin() {
        return typename DoubleLinkedList::IteratorType(_mHeadNode.next);
    }

    typename DoubleLinkedList::ConstIteratorType
    begin() const {
        return typename DoubleLinkedList::ConstIteratorType(_mHeadNode.next);
    }

    typename DoubleLinkedList::IteratorType
    end() {
        return typename DoubleLinkedList::IteratorType(&_mHeadNode);
    }

    typename DoubleLinkedList::ConstIteratorType
    end() const {
        typename DoubleLinkedList::ConstIteratorType(&_mHeadNode);
    }


protected:
    _Node _mHeadNode;
    typename DoubleLinkedList::SizeType _mSize;
};

}
#endif