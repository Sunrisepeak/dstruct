// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef __LINKED_LIST_HPP__DSTRUCT
#define __LINKED_LIST_HPP__DSTRUCT

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
        _Node::init(&_mHeadNode);
        _mSize = 0;
    }

    // copy/move-action impl in subclass
    _LinkedList(const _LinkedList &list) = delete;
    _LinkedList & operator=(const _LinkedList &list) = delete;

    DSTRUCT_MOVE_SEMANTICS(_LinkedList) {
        // 1.clear
        while (!_LinkedList::empty()) pop_front();
        // 2.only move data
        _LinkedList::_mHeadNode = ds._mHeadNode;
        _LinkedList::_mSize = ds._mSize;
        // 3.spec move impl in subclass
        // 4.reset
        _Node::init(&(ds._mHeadNode));
        ds._mSize = 0;

        return *this;
    }

    ~_LinkedList() {
        while (!empty()) {
            pop_front();
        }
    }

public: // base op

    bool empty() const {
        // Note: when _mSize == 1 ->  _mHeadNode.link.next -> _mHeadNode.link
        if (_mSize == 0) {
            DSTRUCT_ASSERT(_mHeadNode.link.next == &(_mHeadNode.link));
            return true;
        }
        return false;
    }

    typename _LinkedList::SizeType size() const {
        return _mSize;
    }

    //T back() const; // impl in sub-class
    //void push_back(const T &obj);
    //void pop_back(); // impl in sub-class

    T front() const {
        return _Node::to_node(_mHeadNode.link.next)->data;
    }

    void push(const T &obj) { push_front(obj); }

    void push_front(const T &obj) {
        // 1. alloc and construct node
        _Node *nPtr = _AllocNode::allocate();
        _mHeadNode.data = obj; // only for contruct nPtr
        dstruct::construct(nPtr, _mHeadNode);
        // 2. add to list
        _Node::LinkType::add(&(_mHeadNode.link), _Node::to_link(nPtr));
        // 3. increase size
        _mSize++;
    }

    void pop() { pop_front(); }

    void pop_front() {
        DSTRUCT_ASSERT(_mSize > 0);
        // 1. get target node's link
        typename _Node::LinkType *lPtr = _mHeadNode.link.next;
        // 2. del target node
        _Node::LinkType::del(_Node::to_link(&_mHeadNode), lPtr);
        // 3. get target node
        _Node *nPtr = _Node::to_node(lPtr);
        // 4. free and decrease size/len
        dstruct::destroy(nPtr);
        _AllocNode::deallocate(nPtr);
        _mSize--;
    }

public: // support it/range-for

    typename _LinkedList::IteratorType
    begin() {
        return typename _LinkedList::IteratorType(_mHeadNode.link.next);
    }

    typename _LinkedList::ConstIteratorType
    begin() const {
        return typename _LinkedList::ConstIteratorType(_mHeadNode.link.next);
    }

    typename _LinkedList::IteratorType
    end() {
        return typename _LinkedList::IteratorType(_Node::to_link(&_mHeadNode));
    }

    typename _LinkedList::ConstIteratorType
    end() const { // headNode-link
        return typename _LinkedList::ConstIteratorType(_Node::to_link(&_mHeadNode));
    }

protected:
    mutable _Node _mHeadNode;
    typename _LinkedList::SizeType _mSize;
};

}
#endif