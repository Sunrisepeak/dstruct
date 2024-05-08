// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef LINKED_LIST_HPP__DSTRUCT
#define LINKED_LIST_HPP__DSTRUCT

#include <core/common.hpp>
#include <core/ds/linked-list/EmbeddedList.hpp>

namespace dstruct {

template <typename T, template <typename> class LinkedListIterator, typename Alloc = dstruct::Alloc>
class _LinkedList : public _DStructTypeSpec<T, Alloc, LinkedListIterator> {

protected:
    //using _Node     = typename _LinkedList::IteratorType::_Node;
    using _Node      = typename LinkedListIterator<T>::_Node;
    using _AllocNode = AllocSpec<_Node, Alloc>;
/*
    // use void link to support doubly/singly link
    union LinkUnion { // TODO: better method?
        struct VoidLink {
            void *next;
            void *prev;
        } _link; // only use in LinkedList
        typename _Node::LinkType link;
    };
*/

public: // big five
    _LinkedList() {
        _Node::init(&mHeadNode_d);
        mSize_d = 0;
    }

    // copy/move-action impl in subclass
    _LinkedList(const _LinkedList &list) = delete;
    _LinkedList & operator=(const _LinkedList &list) = delete;

    DSTRUCT_MOVE_SEMANTICS(_LinkedList) {
        // 1._clear
        _clear();
        // 2.only move data
        _LinkedList::mHeadNode_d = ds.mHeadNode_d;
        _LinkedList::mSize_d = ds.mSize_d;
        // 3.spec move impl in subclass
        // 4.reset
        _Node::init(&(ds.mHeadNode_d));
        ds.mSize_d = 0;

        return *this;
    }

    ~_LinkedList() {
        _clear();
    }

public: // Capacity

    bool empty() const {
        // Note: when mSize_d == 1 ->  mHeadNode_d.link.next -> mHeadNode_d.link
        if (mSize_d == 0) {
            DSTRUCT_ASSERT(mHeadNode_d.link.next == &(mHeadNode_d.link));
            return true;
        }
        return false;
    }

    typename _LinkedList::SizeType size() const {
        return mSize_d;
    }

public: // Access
    T front() const {
        return _Node::to_node(mHeadNode_d.link.next)->data;
    }

public: // Modifiers
    void push(const T &obj) {
        push_front(obj);
    }

    void push_front(const T &obj) {
        // 1. alloc and construct node
        _Node *nPtr = _AllocNode::allocate();
        mHeadNode_d.data = obj; // only for contruct nPtr
        dstruct::construct(nPtr, mHeadNode_d);
        // 2. add to list
        _Node::LinkType::add(&(mHeadNode_d.link), _Node::to_link(nPtr));
        // 3. increase size
        mSize_d++;
    }

    void pop() {
        pop_front();
    }

    void pop_front() {
        DSTRUCT_ASSERT(mSize_d > 0);
        // 1. get target node's link
        typename _Node::LinkType *lPtr = mHeadNode_d.link.next;
        // 2. del target node
        _Node::LinkType::del(_Node::to_link(&mHeadNode_d), lPtr);
        // 3. get target node
        _Node *nPtr = _Node::to_node(lPtr);
        // 4. free and decrease size/len
        dstruct::destroy(nPtr);
        _AllocNode::deallocate(nPtr);
        mSize_d--;
    }

public: // support it/range-for

    typename _LinkedList::IteratorType
    begin() {
        return typename _LinkedList::IteratorType(mHeadNode_d.link.next);
    }

    typename _LinkedList::ConstIteratorType
    begin() const {
        return typename _LinkedList::ConstIteratorType(mHeadNode_d.link.next);
    }

    typename _LinkedList::IteratorType
    end() {
        return typename _LinkedList::IteratorType(_Node::to_link(&mHeadNode_d));
    }

    typename _LinkedList::ConstIteratorType
    end() const { // headNode-link
        return typename _LinkedList::ConstIteratorType(_Node::to_link(&mHeadNode_d));
    }

protected:
    mutable _Node mHeadNode_d;
    typename _LinkedList::SizeType mSize_d;

    void _clear() {
        while (!empty()) {
            pop_front();
        }
    }
};

}
#endif