// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

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
        const _BinaryTreeIterator<typename RemoveConst<T>::Type> &obj,
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

template <typename T, typename Alloc>
class BinaryTree : public _DStructTypeSpec<T, Alloc, _BinaryTreeIterator> {

public: // type
    enum TraversalType : uint8_t {
        PreOrder,
        InOrder,
        PostOrder,
    };

protected:
    using _Node      = EmbeddedBinaryTreeNode<T>; 
    using _AllocNode = AllocSpec<_Node, Alloc>;

public:
    static _Node * copy_tree(_Node *root) {
        if (!root)
            return nullptr;

        _Node *newRoot = _AllocNode::allocate();
        newRoot->data = root->data;
        newRoot->parent = nullptr;

        newRoot->left = copy_tree(root->left);
        if (newRoot->left)
            newRoot->left->parent = newRoot;

        newRoot->right = copy_tree(root->right);
        if (newRoot->right)
            newRoot->right->parent = newRoot;

        return newRoot;
    }

    static typename _Node::LinkType * first_node(typename _Node::LinkType *root, TraversalType ttype = TraversalType::InOrder) {
        auto first = root;
        if (ttype != TraversalType::PreOrder) {
            while (first->left != nullptr) {
                first = first->left;
            }
        }
        return first;
    }
};

} // namespace tree

} // namespace dstruct

#endif