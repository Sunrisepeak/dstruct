// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef SINGLE_LINKED_LIST_HPP__DSTRUCT
#define SINGLE_LINKED_LIST_HPP__DSTRUCT

#include <core/common.hpp>
#include <core/ds/linked-list/EmbeddedList.hpp>
#include <core/ds/linked-list/LinkedList.hpp>

namespace dstruct {

template <typename T>
class _SinglyLinkListIterator : public DStructIteratorTypeSpec<T> {
public:
    using _Node  = _EmbeddedListNode<T, _SinglyLink>;
private:
    using Self = _SinglyLinkListIterator;
public: // big five
    _SinglyLinkListIterator(typename _Node::LinkType *linkPtr) {
        __sync(linkPtr);
    }
public: // ForwardIterator
    Self& operator++() { 
        __sync(_mLinkPtr->next);
        return *this;
    };
    Self operator++(int) {
        auto oldLinkPtr = _mLinkPtr;
        __sync(_mLinkPtr->next);
        return oldLinkPtr;
    };
private:
    // update _mLinkPtr and _mPointer
    void __sync(typename _Node::LinkType *ptr) {
        _mLinkPtr = ptr;
        Self::_mPointer = &(_Node::to_node(_mLinkPtr)->data);
    }
protected:
    typename _Node::LinkType *_mLinkPtr;
};

template <typename T, typename Alloc = dstruct::Alloc>
class SinglyLinkedList : public _LinkedList<T, _SinglyLinkListIterator, Alloc> {
protected:
    using _List = _LinkedList<T, _SinglyLinkListIterator, Alloc>;
    using typename _List::_Node;
    using typename _List::_AllocNode;
    using _List::_mHeadNode;
    using _List::_mSize;

public: // big five
    SinglyLinkedList() : _List(), _mTailNodePtr { &_mHeadNode } {

    }

    SinglyLinkedList(size_t n, const T &obj) : SinglyLinkedList() {
        while (n--) {
            push_back(obj);
        }
    }

    DSTRUCT_COPY_SEMANTICS(SinglyLinkedList) {
        clear();
        // copy
        if (ds._mSize != 0) {
            auto linkPtr =  ds._mHeadNode.link.next;
            auto nextLinkPtr = linkPtr->next;
            auto nodePtr = _Node::to_node(linkPtr);
            while (nextLinkPtr != &(ds._mHeadNode.link)) {
                push_back(nodePtr->data);
                linkPtr = nextLinkPtr;
                nextLinkPtr = nextLinkPtr->next;
                auto nodePtr = _Node::to_node(linkPtr);
            }
            push_back(nodePtr->data);
            _mTailNodePtr = nodePtr;
        }
        return *this;
    }

    DSTRUCT_MOVE_SEMANTICS(SinglyLinkedList) {
        // move list
        _List::operator=(dstruct::move(ds));
        _mTailNodePtr = ds._mTailNodePtr;
        _mTailNodePtr->link.next = _Node::to_link(&_mHeadNode);
        // reset
        ds._mTailNodePtr = &(ds._mHeadNode);
        return *this;
    }

    ~SinglyLinkedList() = default;

public:
    T back() const {
        DSTRUCT_ASSERT(_mSize > 0);
        return _mTailNodePtr->data;
    }

    void push_back(const T &obj) {
        // 1. alloc and construct node
        _Node *nPtr = _AllocNode::allocate();
        this->_mHeadNode.data = obj; // only for contruct nPtr
        dstruct::construct(nPtr, this->_mHeadNode);
        // 2. add to list
        _Node::LinkType::add(_Node::to_link(_mTailNodePtr), _Node::to_link(nPtr));
        // 3. increase size
        _mSize++;
        // 4. update _mTailNodePtr
        _mTailNodePtr = nPtr;
    }

    void push_front(const T &obj) {
        _List::push_front(obj);
        if (_mSize == 1)
            _mTailNodePtr = _Node::to_node(_mHeadNode.link.next);
    }

    void pop_front() {
        _List::pop_front();
        if (_mSize == 0)
            _mTailNodePtr = &_mHeadNode;
    }

    void clear() {
        _List::_clear();
        _mTailNodePtr = &_mHeadNode;
    }

public: // low efficient
    void _pop_back() {
        DSTRUCT_ASSERT(_mSize > 0);
        auto link = _Node::to_link(&_mHeadNode);
        while (link->next != _Node::to_link(_mTailNodePtr)) {
            link = link->next;
        }
        //free and decrease size/len
        dstruct::destroy(_mTailNodePtr);
        _AllocNode::deallocate(_mTailNodePtr);
        this->_mSize--;
        // update _mTailNodePtr and link
        _mTailNodePtr = _Node::to_node(link);
        _mTailNodePtr->link.next = _Node::to_link(&_mHeadNode);
    }

protected:
    _Node *_mTailNodePtr;
};

}
#endif