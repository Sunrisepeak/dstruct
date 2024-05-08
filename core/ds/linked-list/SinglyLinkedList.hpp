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
        __sync(mLinkPtr_d->next);
        return *this;
    };
    Self operator++(int) {
        auto oldLinkPtr = mLinkPtr_d;
        __sync(mLinkPtr_d->next);
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
class SinglyLinkedList : public _LinkedList<T, _SinglyLinkListIterator, Alloc> {
protected:
    using _List = _LinkedList<T, _SinglyLinkListIterator, Alloc>;
    using typename _List::_Node;
    using typename _List::_AllocNode;
    using _List::mHeadNode_d;
    using _List::mSize_d;

public: // big five
    SinglyLinkedList() : _List(), mTailNodePtr_d { &mHeadNode_d } {

    }

    SinglyLinkedList(size_t n, const T &obj) : SinglyLinkedList() {
        while (n--) {
            push_back(obj);
        }
    }

    DSTRUCT_COPY_SEMANTICS(SinglyLinkedList) {
        clear();
        // copy
        if (ds.mSize_d != 0) {
            auto linkPtr =  ds.mHeadNode_d.link.next;
            auto nextLinkPtr = linkPtr->next;
            auto nodePtr = _Node::to_node(linkPtr);
            while (nextLinkPtr != &(ds.mHeadNode_d.link)) {
                push_back(nodePtr->data);
                linkPtr = nextLinkPtr;
                nextLinkPtr = nextLinkPtr->next;
                auto nodePtr = _Node::to_node(linkPtr);
            }
            push_back(nodePtr->data);
            mTailNodePtr_d = nodePtr;
        }
        return *this;
    }

    DSTRUCT_MOVE_SEMANTICS(SinglyLinkedList) {
        // move list
        _List::operator=(dstruct::move(ds));
        mTailNodePtr_d = ds.mTailNodePtr_d;
        mTailNodePtr_d->link.next = _Node::to_link(&mHeadNode_d);
        // reset
        ds.mTailNodePtr_d = &(ds.mHeadNode_d);
        return *this;
    }

    ~SinglyLinkedList() = default;

public:
    T back() const {
        DSTRUCT_ASSERT(mSize_d > 0);
        return mTailNodePtr_d->data;
    }

    void push_back(const T &obj) {
        // 1. alloc and construct node
        _Node *nPtr = _AllocNode::allocate();
        this->mHeadNode_d.data = obj; // only for contruct nPtr
        dstruct::construct(nPtr, this->mHeadNode_d);
        // 2. add to list
        _Node::LinkType::add(_Node::to_link(mTailNodePtr_d), _Node::to_link(nPtr));
        // 3. increase size
        mSize_d++;
        // 4. update mTailNodePtr_d
        mTailNodePtr_d = nPtr;
    }

    void push_front(const T &obj) {
        _List::push_front(obj);
        if (mSize_d == 1)
            mTailNodePtr_d = _Node::to_node(mHeadNode_d.link.next);
    }

    void pop_front() {
        _List::pop_front();
        if (mSize_d == 0)
            mTailNodePtr_d = &mHeadNode_d;
    }

    void clear() {
        _List::_clear();
        mTailNodePtr_d = &mHeadNode_d;
    }

public: // low efficient
    void _pop_back() {
        DSTRUCT_ASSERT(mSize_d > 0);
        auto link = _Node::to_link(&mHeadNode_d);
        while (link->next != _Node::to_link(mTailNodePtr_d)) {
            link = link->next;
        }
        //free and decrease size/len
        dstruct::destroy(mTailNodePtr_d);
        _AllocNode::deallocate(mTailNodePtr_d);
        this->mSize_d--;
        // update mTailNodePtr_d and link
        mTailNodePtr_d = _Node::to_node(link);
        mTailNodePtr_d->link.next = _Node::to_link(&mHeadNode_d);
    }

protected:
    _Node *mTailNodePtr_d;
};

}
#endif