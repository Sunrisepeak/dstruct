// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef SINGLE_LINKED_LIST_HPP_DSTRUCT
#define SINGLE_LINKED_LIST_HPP_DSTRUCT

#include <core/common.hpp>
#include <core/ds/linked-list/EmbeddedList.hpp>
#include <core/ds/linked-list/LinkedList.hpp>

namespace dstruct {

template <typename T>
class SinglyLinkListIterator_ : public DStructIteratorTypeSpec<T> {
public:
    using Node_  = EmbeddedListNode_<T, SinglyLink_>;
private:
    using Self = SinglyLinkListIterator_;
public: // big five
    SinglyLinkListIterator_(typename Node_::LinkType *linkPtr) {
        _sync(linkPtr);
    }
public: // ForwardIterator
    Self& operator++() { 
        _sync(mLinkPtr_d->next);
        return *this;
    };
    Self operator++(int) {
        auto oldLinkPtr = mLinkPtr_d;
        _sync(mLinkPtr_d->next);
        return oldLinkPtr;
    };
private:
    // update mLinkPtr_d and mPointer_d
    void _sync(typename Node_::LinkType *ptr) {
        mLinkPtr_d = ptr;
        Self::mPointer_d = &(Node_::to_node(mLinkPtr_d)->data);
    }
protected:
    typename Node_::LinkType *mLinkPtr_d;
};

template <typename T, typename Alloc = dstruct::Alloc>
class SinglyLinkedList : public LinkedList_<T, SinglyLinkListIterator_, Alloc> {
protected:
    using List_ = LinkedList_<T, SinglyLinkListIterator_, Alloc>;
    using typename List_::Node_;
    using typename List_::AllocNode_;
    using List_::mHeadNode_d;
    using List_::mSize_d;

public: // big five
    SinglyLinkedList() : List_(), mTailNodePtr_d { &mHeadNode_d } {

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
            auto nodePtr = Node_::to_node(linkPtr);
            while (nextLinkPtr != &(ds.mHeadNode_d.link)) {
                push_back(nodePtr->data);
                linkPtr = nextLinkPtr;
                nextLinkPtr = nextLinkPtr->next;
                auto nodePtr = Node_::to_node(linkPtr);
            }
            push_back(nodePtr->data);
            mTailNodePtr_d = nodePtr;
        }
        return *this;
    }

    DSTRUCT_MOVE_SEMANTICS(SinglyLinkedList) {
        // move list
        List_::operator=(dstruct::move(ds));
        mTailNodePtr_d = ds.mTailNodePtr_d;
        mTailNodePtr_d->link.next = Node_::to_link(&mHeadNode_d);
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
        Node_ *nPtr = AllocNode_::allocate();
        this->mHeadNode_d.data = obj; // only for contruct nPtr
        dstruct::construct(nPtr, this->mHeadNode_d);
        // 2. add to list
        Node_::LinkType::add(Node_::to_link(mTailNodePtr_d), Node_::to_link(nPtr));
        // 3. increase size
        mSize_d++;
        // 4. update mTailNodePtr_d
        mTailNodePtr_d = nPtr;
    }

    void push_front(const T &obj) {
        List_::push_front(obj);
        if (mSize_d == 1)
            mTailNodePtr_d = Node_::to_node(mHeadNode_d.link.next);
    }

    void pop_front() {
        List_::pop_front();
        if (mSize_d == 0)
            mTailNodePtr_d = &mHeadNode_d;
    }

    void clear() {
        List_::_clear();
        mTailNodePtr_d = &mHeadNode_d;
    }

public: // low efficient
    void _pop_back() {
        DSTRUCT_ASSERT(mSize_d > 0);
        auto link = Node_::to_link(&mHeadNode_d);
        while (link->next != Node_::to_link(mTailNodePtr_d)) {
            link = link->next;
        }
        //free and decrease size/len
        dstruct::destroy(mTailNodePtr_d);
        AllocNode_::deallocate(mTailNodePtr_d);
        this->mSize_d--;
        // update mTailNodePtr_d and link
        mTailNodePtr_d = Node_::to_node(link);
        mTailNodePtr_d->link.next = Node_::to_link(&mHeadNode_d);
    }

protected:
    Node_ *mTailNodePtr_d;
};

}
#endif