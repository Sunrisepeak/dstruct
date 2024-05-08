// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef DOUBLE_LINKED_LIST_HPP__DSTRUCT
#define DOUBLE_LINKED_LIST_HPP__DSTRUCT

#include <core/common.hpp>
#include <core/ds/linked-list/EmbeddedList.hpp>
#include <core/ds/linked-list/LinkedList.hpp>

namespace dstruct {

template <typename T>
class _DoublyLinkListIterator : public DStructIteratorTypeSpec<T, BidirectionalIterator> {
public:
    using _Node  = _EmbeddedListNode<T, _DoublyLink>;
private:
    using Self = _DoublyLinkListIterator;

public: // big five
    _DoublyLinkListIterator(typename _Node::LinkType *linkPtr) {
        __sync(linkPtr);
    }

public: // ForwardIterator
    Self& operator++() { 
        __sync(mLinkPtr_d->next);
        return *this;
    };
    Self operator++(int) {
        auto oldLinkPtr = mLinkPtr_d;
        __sync(mLinkPtr_d->next);
        return oldLinkPtr;
    };
public: // BidirectionalIterator
    Self& operator--() {
        __sync(mLinkPtr_d->prev);
        return *this;
    };
    Self operator--(int) {
        auto oldLinkPtr = mLinkPtr_d;
        __sync(mLinkPtr_d->prev);
        return oldLinkPtr;
    };
private:
    // update mLinkPtr_d and mPointer_d
    void __sync(typename _Node::LinkType *ptr) {
        mLinkPtr_d = ptr;
        Self::mPointer_d = &(_Node::to_node(mLinkPtr_d)->data);
    }
protected:
    typename _Node::LinkType *mLinkPtr_d;
};

template <typename T, typename Alloc = dstruct::Alloc>
class DoublyLinkedList : public _LinkedList<T, _DoublyLinkListIterator, Alloc> {

protected:
    using _List = _LinkedList<T, _DoublyLinkListIterator, Alloc>;
    using typename _List::_Node;
    using typename _List::_AllocNode;
    using _List::mSize_d;
    using _List::mHeadNode_d;

public: // big five
    // use _List to complete
    DoublyLinkedList() = default;
    DoublyLinkedList(size_t n, const T &obj) : DoublyLinkedList() { while(n--) push_back(obj); }

    DSTRUCT_COPY_SEMANTICS(DoublyLinkedList) {
        clear();
        // copy
        for (auto it = ds.begin(); it != ds.end(); it++) {
            push_back(*it);
        }
        return *this;
    }

    DSTRUCT_MOVE_SEMANTICS(DoublyLinkedList) {
        // move list data
        _List::operator=(dstruct::move(ds));
        // update link: first-data and last-data point to new headNode
        auto headLinkPtr = _Node::to_link(&mHeadNode_d);
        mHeadNode_d.link.prev->next = headLinkPtr;
        mHeadNode_d.link.next->prev = headLinkPtr;
        return *this;
    }

    ~DoublyLinkedList() = default;

public: // base op

    T back() const {
        DSTRUCT_ASSERT(mSize_d > 0);
        return _Node::to_node(mHeadNode_d.link.prev)->data;
    }

    void push_back(const T &obj) {
        // 1. alloc memory
        _Node *nPtr = _AllocNode::allocate();
        // 2. construct node
        mHeadNode_d.data = obj; // only for contruct nPtr
        dstruct::construct(nPtr, mHeadNode_d);
        // 3. add node to list
        _Node::LinkType::add(mHeadNode_d.link.prev, _Node::to_link(nPtr));
        // 4. increase size
        mSize_d++;
    }

    void pop_back() {
        DSTRUCT_ASSERT(mSize_d > 0);
        // 1. get node link ptr
        typename _Node::LinkType *lPtr = mHeadNode_d.link.prev;
        // 2. del node from list
        _Node::LinkType::del(lPtr->prev, lPtr);
        // 3. get target node
        _Node *nPtr = _Node::to_node(lPtr);
        // 4. free and decrease size/len
        dstruct::destroy(nPtr);
        _AllocNode::deallocate(nPtr);
        mSize_d--;
    }

    void clear() {
        _List::_clear();
    }
};

}
#endif