#ifndef __BINARY_TREE_HPP__DSTRUCT
#define __BINARY_TREE_HPP__DSTRUCT

#include <core/common.hpp>
#include <core/ds/tree/EmbeddedBinaryTree.hpp>

namespace dstruct {

namespace tree {

template <typename T>
class _BinaryTreeIterator : public DStructIteratorTypeSpec<T> {

private:
    using __Self = _BinaryTreeIterator;
protected:
    using _Node = EmbeddedBinaryTreeNode<T>;
public:
    using NextFunc = typename _Node::LinkType * (*)(typename _Node::LinkType *);

public:
    _BinaryTreeIterator(_Node *nPtr, NextFunc *next) :
        _mTreeNodePtr { nPtr }, _mNext { next } { }

public: // ForwardIterator
    __Self& operator++() {
        _mNext(_Node::to_link(_mTreeNodePtr));
        __sync();
        return *this;
    }

    __Self operator++(int) {
        __Self old = *this;
        _mNext(_Node::to_link(_mTreeNodePtr));
        __sync();
        return old;
    }

private:
    void __sync() {
        __Self::_mPointer = &(_mTreeNodePtr->data);
    }

protected:
    _Node *_mTreeNodePtr;
    NextFunc *_mNext;
};

/*
struct BinaryTree {
    template <typename BinaryTreeIteratorType, typename Callback>
    static void preorder_traversal(BinaryTreeIteratorType begin, Callback &cb) {

    }

    template <typename BinaryTreeIteratorType, typename Callback>
    static void inorder_traversal(BinaryTreeIteratorType begin, Callback &cb) {

    }

    template <typename BinaryTreeIteratorType, typename Callback>
    static void postorder_traversal(BinaryTreeIteratorType begin, Callback &cb) {

    }

};
*/

} // namespace tree

} // namespace dstruct

#endif