// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef __DOUBLE_LINKED_LIST_HPP__DSTRUCT
#define __DOUBLE_LINKED_LIST_HPP__DSTRUCT

#include <core/common.hpp>
#include <core/ds/linked-list/EmbeddedList.hpp>
#include <core/ds/linked-list/LinkedList.hpp>

namespace dstruct {

template <typename T>
class _DoublyLinkListIterator : public DStructIteratorTypeSpec<T, BidirectionalIterator> {
public:
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

template <typename T, typename Alloc = dstruct::Alloc>
class DoublyLinkedList : public _LinkedList<T, _DoublyLinkListIterator, Alloc> {

protected:
    using _List = _LinkedList<T, _DoublyLinkListIterator, Alloc>;
    using typename _List::_Node;
    using typename _List::_AllocNode;
    using _List::_mSize;
    using _List::_mHeadNode;

public: // big five
    // use _List to complete
    DoublyLinkedList() = default;
    DoublyLinkedList(size_t n, const T &obj) : DoublyLinkedList() { while(n--) push_back(obj); }

    DoublyLinkedList(const DoublyLinkedList &list) : DoublyLinkedList() { *this = list; }
    DoublyLinkedList & operator=(const DoublyLinkedList &list) {
        while (!_List::empty()) pop_back(); // clear
        // copy
        for (auto it = list.begin(); it != list.end(); it++) {
            push_back(*it);
        }
        return *this;
    }

    DoublyLinkedList(DoublyLinkedList &&list) : DoublyLinkedList() { *this = dstruct::move(list); }
    DoublyLinkedList & operator=(DoublyLinkedList &&list) {
        // move list data
        _List::operator=(dstruct::move(list));
        // update link: first-data and last-data point to new headNode
        auto headLinkPtr = _Node::to_link(&_mHeadNode);
        _mHeadNode.link.prev->next = headLinkPtr;
        _mHeadNode.link.next->prev = headLinkPtr;
        return *this;
    }

    ~DoublyLinkedList() = default;

public: // base op

    T back() const {
        DSTRUCT_ASSERT(_mSize > 0);
        return _Node::to_node(_mHeadNode.link.prev)->data;
    }

    void push_back(const T &obj) {
        // 1. alloc memory
        _Node *nPtr = _AllocNode::allocate();
        // 2. construct node
        _mHeadNode.data = obj; // only for contruct nPtr
        dstruct::construct(nPtr, _mHeadNode);
        // 3. add node to list
        _Node::LinkType::add(_mHeadNode.link.prev, _Node::to_link(nPtr));
        // 4. increase size
        _mSize++;
    }

    void pop_back() {
        DSTRUCT_ASSERT(_mSize > 0);
        // 1. get node link ptr
        typename _Node::LinkType *lPtr = _mHeadNode.link.prev;
        // 2. del node from list
        _Node::LinkType::del(lPtr->prev, lPtr);
        // 3. get target node
        _Node *nPtr = _Node::to_node(lPtr);
        // 4. free and decrease size/len
        dstruct::destory(nPtr);
        _AllocNode::deallocate(nPtr);
        _mSize--;
    }
};

}
#endif