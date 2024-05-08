// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef DOUBLE_LINKED_LIST_HPP_DSTRUCT
#define DOUBLE_LINKED_LIST_HPP_DSTRUCT

#include <core/common.hpp>
#include <core/ds/linked-list/EmbeddedList.hpp>
#include <core/ds/linked-list/LinkedList.hpp>

namespace dstruct {

template <typename T>
class DoublyLinkListIterator_ : public DStructIteratorTypeSpec<T, BidirectionalIterator> {
public:
    using Node_  = EmbeddedListNode_<T, DoublyLink_>;
private:
    using Self = DoublyLinkListIterator_;

public: // big five
    DoublyLinkListIterator_(typename Node_::LinkType *linkPtr) {
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
public: // BidirectionalIterator
    Self& operator--() {
        _sync(mLinkPtr_d->prev);
        return *this;
    };
    Self operator--(int) {
        auto oldLinkPtr = mLinkPtr_d;
        _sync(mLinkPtr_d->prev);
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
class DoublyLinkedList : public LinkedList_<T, DoublyLinkListIterator_, Alloc> {

protected:
    using List_ = LinkedList_<T, DoublyLinkListIterator_, Alloc>;
    using typename List_::Node_;
    using typename List_::AllocNode_;
    using List_::mSize_d;
    using List_::mHeadNode_d;

public: // big five
    // use List_ to complete
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
        List_::operator=(dstruct::move(ds));
        // update link: first-data and last-data point to new headNode
        auto headLinkPtr = Node_::to_link(&mHeadNode_d);
        mHeadNode_d.link.prev->next = headLinkPtr;
        mHeadNode_d.link.next->prev = headLinkPtr;
        return *this;
    }

    ~DoublyLinkedList() = default;

public: // base op

    T back() const {
        DSTRUCT_ASSERT(mSize_d > 0);
        return Node_::to_node(mHeadNode_d.link.prev)->data;
    }

    void push_back(const T &obj) {
        // 1. alloc memory
        Node_ *nPtr = AllocNode_::allocate();
        // 2. construct node
        mHeadNode_d.data = obj; // only for contruct nPtr
        dstruct::construct(nPtr, mHeadNode_d);
        // 3. add node to list
        Node_::LinkType::add(mHeadNode_d.link.prev, Node_::to_link(nPtr));
        // 4. increase size
        mSize_d++;
    }

    void pop_back() {
        DSTRUCT_ASSERT(mSize_d > 0);
        // 1. get node link ptr
        typename Node_::LinkType *lPtr = mHeadNode_d.link.prev;
        // 2. del node from list
        Node_::LinkType::del(lPtr->prev, lPtr);
        // 3. get target node
        Node_ *nPtr = Node_::to_node(lPtr);
        // 4. free and decrease size/len
        dstruct::destroy(nPtr);
        AllocNode_::deallocate(nPtr);
        mSize_d--;
    }

    void clear() {
        List_::_clear();
    }
};

}
#endif