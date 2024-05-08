// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef LINKED_LIST_HPP_DSTRUCT
#define LINKED_LIST_HPP_DSTRUCT

#include <core/common.hpp>
#include <core/ds/linked-list/EmbeddedList.hpp>

namespace dstruct {

template <typename T, template <typename> class LinkedListIterator, typename Alloc = dstruct::Alloc>
class LinkedList_ : public DStructTypeSpec_<T, Alloc, LinkedListIterator> {

protected:
    //using Node_     = typename LinkedList_::IteratorType::Node_;
    using Node_      = typename LinkedListIterator<T>::Node_;
    using AllocNode_ = AllocSpec<Node_, Alloc>;
/*
    // use void link to support doubly/singly link
    union LinkUnion { // TODO: better method?
        struct VoidLink {
            void *next;
            void *prev;
        } _link; // only use in LinkedList
        typename Node_::LinkType link;
    };
*/

public: // big five
    LinkedList_() {
        Node_::init(&mHeadNode_d);
        mSize_d = 0;
    }

    // copy/move-action impl in subclass
    LinkedList_(const LinkedList_ &list) = delete;
    LinkedList_ & operator=(const LinkedList_ &list) = delete;

    DSTRUCT_MOVE_SEMANTICS(LinkedList_) {
        // 1._clear
        _clear();
        // 2.only move data
        LinkedList_::mHeadNode_d = ds.mHeadNode_d;
        LinkedList_::mSize_d = ds.mSize_d;
        // 3.spec move impl in subclass
        // 4.reset
        Node_::init(&(ds.mHeadNode_d));
        ds.mSize_d = 0;

        return *this;
    }

    ~LinkedList_() {
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

    typename LinkedList_::SizeType size() const {
        return mSize_d;
    }

public: // Access
    T front() const {
        return Node_::to_node(mHeadNode_d.link.next)->data;
    }

public: // Modifiers
    void push(const T &obj) {
        push_front(obj);
    }

    void push_front(const T &obj) {
        // 1. alloc and construct node
        Node_ *nPtr = AllocNode_::allocate();
        mHeadNode_d.data = obj; // only for contruct nPtr
        dstruct::construct(nPtr, mHeadNode_d);
        // 2. add to list
        Node_::LinkType::add(&(mHeadNode_d.link), Node_::to_link(nPtr));
        // 3. increase size
        mSize_d++;
    }

    void pop() {
        pop_front();
    }

    void pop_front() {
        DSTRUCT_ASSERT(mSize_d > 0);
        // 1. get target node's link
        typename Node_::LinkType *lPtr = mHeadNode_d.link.next;
        // 2. del target node
        Node_::LinkType::del(Node_::to_link(&mHeadNode_d), lPtr);
        // 3. get target node
        Node_ *nPtr = Node_::to_node(lPtr);
        // 4. free and decrease size/len
        dstruct::destroy(nPtr);
        AllocNode_::deallocate(nPtr);
        mSize_d--;
    }

public: // support it/range-for

    typename LinkedList_::IteratorType
    begin() {
        return typename LinkedList_::IteratorType(mHeadNode_d.link.next);
    }

    typename LinkedList_::ConstIteratorType
    begin() const {
        return typename LinkedList_::ConstIteratorType(mHeadNode_d.link.next);
    }

    typename LinkedList_::IteratorType
    end() {
        return typename LinkedList_::IteratorType(Node_::to_link(&mHeadNode_d));
    }

    typename LinkedList_::ConstIteratorType
    end() const { // headNode-link
        return typename LinkedList_::ConstIteratorType(Node_::to_link(&mHeadNode_d));
    }

protected:
    mutable Node_ mHeadNode_d;
    typename LinkedList_::SizeType mSize_d;

    void _clear() {
        while (!empty()) {
            pop_front();
        }
    }
};

}
#endif