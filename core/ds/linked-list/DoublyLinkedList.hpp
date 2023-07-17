#ifndef __DOUBLE_LINKED_LIST_HPP__DSTRUCT
#define __DOUBLE_LINKED_LIST_HPP__DSTRUCT

#include <core/common.hpp>

#include <core/ds/linked-list/EmbeddedList.hpp>

namespace dstruct {

template<typename T>
class _DoublyLinkListIterator : public DStructIteratorTypeSpec<T, BidirectionalIterator> {
protected:
    using _Node  = _EmbeddedListNode<T, _DoublyLink>;
private:
    using __Self = _DoublyLinkListIterator;

public: // big five
    _DoublyLinkListIterator(typename _Node::LinkType *linkPtr) {
        __sync(linkPtr);
    }

public: // ForwardIterator
    __Self& operator++() { 
        __sync(_mLinkPtr->next);
        return *this;
    };
    __Self operator++(int) {
        auto oldLinkPtr = _mLinkPtr;
        __sync(_mLinkPtr->next);
        return oldLinkPtr;
    };
public: // BidirectionalIterator
    __Self& operator--() {
        __sync(_mLinkPtr->prev);
        return *this;
    };
    __Self operator--(int) {
        auto oldLinkPtr = _mLinkPtr;
        __sync(_mLinkPtr->prev);
        return oldLinkPtr;
    };
private:
    // update _mLinkPtr and _mPointer
    void __sync(typename _Node::LinkType *ptr) {
        _mLinkPtr = ptr;
        __Self::_mPointer = &(_Node::to_node(_mLinkPtr)->data);
    }
protected:
    typename _Node::LinkType *_mLinkPtr;
};

template<typename T, typename Alloc = port::Alloc>
class DoublyLinkedList : public DStructTypeSpec<T, Alloc, _DoublyLinkListIterator> {

protected:
    using _Node      = _EmbeddedListNode<T, _DoublyLink>; 
    using _AllocNode = DSAlloc<_Node, Alloc>;


public: // big five
    DoublyLinkedList() {
        //_mHeadNode.link.next = _mHeadNode.link.prev = &_mHeadNode.link;
        _Node::init(&_mHeadNode);
        _mSize = 0;
    }

    DoublyLinkedList(const T &obj, size_t n) : DoublyLinkedList() {
        for (int i = 0; i < n; i++) {
            push_back(obj);
        }
    }

    ~DoublyLinkedList() {
        while (!empty()) {
            pop_back();
        }
    }

public: // base op
    
    bool empty() const {
        // Note: when _mSize == 1 ->  _mHeadNode.link.next == _mHeadNode.link.prev
        if (_mSize == 0) {
            DSTRUCT_ASSERT(_mHeadNode.link.next == _mHeadNode.link.prev);
            return true;
        }
        return false;
    }

    typename DoublyLinkedList::SizeType size() const {
        return _mSize;
    }

    T back() const {
        DSTRUCT_ASSERT(_mSize > 0);
        return _Node::to_node(_mHeadNode.link.prev)->data;
    }

    T front() const {
        return _Node::to_node(_mHeadNode.link.next)->data;
    }

    void push(const T &obj) {
        push_back();
    }

    void push_back(const T &obj) {
        // 1. alloc and construct node
        _Node *nPtr = _AllocNode::allocate();
        _mHeadNode.data = obj; // only for contruct nPtr
        dstruct::construct(nPtr, _mHeadNode);
        /*
        // 2. nPtr point to list
        nPtr->next = &_mHeadNode;
        nPtr->prev = _mHeadNode.link.prev;
        // 3. list point to new node
        _mHeadNode.link.prev->next = nPtr;
        _mHeadNode.link.prev = nPtr;
        */
        _Node::LinkType::add(_mHeadNode.link.prev, _Node::to_link(nPtr));
        // 4. increase size
        _mSize++;
    }

    void push_front(const T &obj) {
        // 1. alloc and construct node
        _Node *nPtr = _AllocNode::allocate();
        _mHeadNode.data = obj; // only for contruct nPtr
        dstruct::construct(nPtr, _mHeadNode);
        // 2. add
        _Node::LinkType::add(&(_mHeadNode.link), _Node::to_link(nPtr));
        // 4. increase size
        _mSize++;
    }

    void pop() {
        pop_back();
    }

    void pop_back() {
        DSTRUCT_ASSERT(_mSize > 0);

        // 1. get node link ptr
        typename _Node::LinkType *lPtr = _mHeadNode.link.prev;
        
        // 2. del node from list
        //nPtr->prev->next = nPtr->next;
        //nPtr->next->prev = nPtr->prev;
        //_mHeadNode.link.prev = nPtr->prev;
        _Node::LinkType::del(lPtr->prev, lPtr);

        // 3. get data
        _Node *nPtr = _Node::to_node(lPtr);
        typename DoublyLinkedList::ValueType data = nPtr->data;
        // 4. free and decrease size/len
        dstruct::destory(nPtr);
        _AllocNode::deallocate(nPtr);
        _mSize--;

        //return data;
    }

    void pop_front() {
        typename _Node::LinkType *lPtr = _mHeadNode.link.next;
        _Node::LinkType::del(lPtr->prev, lPtr);
        // 3. get data
        _Node *nPtr = _Node::to_node(lPtr);
        // 4. free and decrease size/len
        dstruct::destory(nPtr);
        _AllocNode::deallocate(nPtr);
        _mSize--;
    }

public: // support it/range-for

    typename DoublyLinkedList::IteratorType
    begin() {
        return typename DoublyLinkedList::IteratorType(_mHeadNode.link.next);
    }

    typename DoublyLinkedList::ConstIteratorType
    begin() const {
        return typename DoublyLinkedList::ConstIteratorType(_mHeadNode.link.next);
    }

    typename DoublyLinkedList::IteratorType
    end() {
        return typename DoublyLinkedList::IteratorType((_mHeadNode.link.next)->prev);
    }

    typename DoublyLinkedList::ConstIteratorType
    end() const { // headNode-link
        return typename DoublyLinkedList::ConstIteratorType((_mHeadNode.link.next)->prev);
    }


protected:
    _Node _mHeadNode;
    typename DoublyLinkedList::SizeType _mSize;
};

}
#endif