#ifndef __BINARY_TREE_HPP__DSTRUCT
#define __BINARY_TREE_HPP__DSTRUCT

#include <core/common.hpp>
#include <core/ds/tree/EmbeddedBinaryTree.hpp>

namespace dstruct {

namespace tree {

template <typename T>
class _BinaryTreeIterator : public DStructIteratorTypeSpec<T> {
    friend class _BinaryTreeIterator<const T>; // for const-it -> it
private:
    using __Self = _BinaryTreeIterator;
protected:
    using _Node = EmbeddedBinaryTreeNode<T>;
public:
    using NextFunc = typename _Node::LinkType * (*)(typename _Node::LinkType *);

public: // bigfive
    _BinaryTreeIterator(typename _Node::LinkType * link, NextFunc nextFunc) :
        _mTreeNodeLink { link }, _mNextFunc { nextFunc } { __sync(); }

    // from it convert to const-it
    _BinaryTreeIterator(const _BinaryTreeIterator<typename types::RemoveConst<T>::Type> &obj)
        : _BinaryTreeIterator(obj._mTreeNodeLink, obj._mNextFunc) { __sync(); }

public: // ForwardIterator
    __Self& operator++() {
        _mTreeNodeLink = _mNextFunc(_mTreeNodeLink);
        __sync();
        return *this;
    }

    __Self operator++(int) {
        __Self old = *this;
        _mTreeNodeLink = _mNextFunc(_mTreeNodeLink);
        __sync();
        return old;
    }

private:
    void __sync() {
        __Self::_mPointer = &(_Node::to_node(_mTreeNodeLink)->data);
    }

protected:
    typename _Node::LinkType *_mTreeNodeLink;
    NextFunc _mNextFunc;
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