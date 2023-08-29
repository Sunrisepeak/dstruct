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
        _mTreeNodeLinkPtr { link }, _mNextFunc { nextFunc } { __sync(); }

    _BinaryTreeIterator(const _BinaryTreeIterator &it) { *this = it; }
    _BinaryTreeIterator & operator=(const _BinaryTreeIterator &it) {
        _mTreeNodeLinkPtr = it._mTreeNodeLinkPtr;
        _mNextFunc = it._mNextFunc;
        __sync();
        return *this;
    }

    _BinaryTreeIterator(_BinaryTreeIterator &&it) { *this = dstruct::move(it); }
    _BinaryTreeIterator & operator=(_BinaryTreeIterator &&it) {
        _mTreeNodeLinkPtr = it._mTreeNodeLinkPtr;
        _mNextFunc = it._mNextFunc;

        // reset
        it._mTreeNodeLinkPtr = nullptr;
        it._mNextFunc = nullptr;
 
        __sync();

        return *this;
    }

    ~_BinaryTreeIterator() {
        _mTreeNodeLinkPtr = nullptr;
        _mNextFunc = nullptr;
    }

    // from it convert to const-it
    _BinaryTreeIterator(
        const _BinaryTreeIterator<typename types::RemoveConst<T>::Type> &obj,
        bool __unused
    ) : _BinaryTreeIterator(obj._mTreeNodeLinkPtr, obj._mNextFunc) {
        //__sync();
    }

public: // ForwardIterator
    __Self& operator++() {
        _mTreeNodeLinkPtr = _mNextFunc(_mTreeNodeLinkPtr);
        __sync();
        return *this;
    }

    __Self operator++(int) {
        __Self old = *this;
        _mTreeNodeLinkPtr = _mNextFunc(_mTreeNodeLinkPtr);
        __sync();
        return old;
    }

private:
    void __sync() {
        __Self::_mPointer = &(_Node::to_node(_mTreeNodeLinkPtr)->data);
    }

protected:
    typename _Node::LinkType *_mTreeNodeLinkPtr;
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