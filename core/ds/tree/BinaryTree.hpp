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
    BinaryTree(_Node *root, size_t size) : _mSize { size }, _mRootPtr { root } { }
/*
    DSTRUCT_COPY_SEMANTICS
    DSTRUCT_MOVE_SEMANTICS
    (Note: impl by subclass)
*/
    virtual ~BinaryTree() {
        if (_mRootPtr) {
            tree::postorder_traversal(&(_mRootPtr->link), [](typename _Node::LinkType *linkPtr) {
                _Node *nPtr = _Node::to_node(linkPtr);
                dstruct::destory(nPtr);
                _AllocNode::deallocate(nPtr);
            });
        }
        _mRootPtr = nullptr;
        _mSize = 0;
    }

public:
    bool empty() const {
        return _mSize == 0;
    }

    size_t size() const {
        return _mSize;
    }

public: // algo
    template <typename Callback>
    void traversal(Callback cb, TraversalType ttype = TraversalType::InOrder) const {

        auto cbWrapper = [&](typename _Node::LinkType *link) {
            typename  BinaryTree::ConstIteratorType::ValueType &data = _Node::to_node(link)->data;
            cb(data);
        };

        switch (ttype) {
            case TraversalType::PreOrder:
                return  tree::preorder_traversal(first_node(_Node::to_link(_mRootPtr), ttype), cbWrapper);
            case TraversalType::InOrder:
                return  tree::inorder_traversal(first_node(_Node::to_link(_mRootPtr), ttype), cbWrapper);
            case TraversalType::PostOrder:
                return  tree::postorder_traversal(first_node(_Node::to_link(_mRootPtr), ttype), cbWrapper);
            default: {
                DSTRUCT_ASSERT(false);
            }
        }

        return  tree::preorder_traversal(first_node(_Node::to_link(_mRootPtr), ttype), cbWrapper);
    }

public: // range-for and iterator

    typename BinaryTree::ConstIteratorType begin(TraversalType ttype = TraversalType::InOrder) const {
        return typename BinaryTree::ConstIteratorType(
            _create_iterator(first_node(_Node::to_link(_mRootPtr), ttype), ttype),
            true
        );
    }

    typename BinaryTree::ConstIteratorType end(TraversalType ttype = TraversalType::InOrder) const {
        return typename BinaryTree::ConstIteratorType(
            _create_iterator(nullptr, ttype),
            true
        );
    }

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

protected:
    size_t _mSize;
    _Node *_mRootPtr;

    void _update_root(typename _Node::LinkType *root) {
        _mRootPtr = _Node::to_node(root);
        if (_mRootPtr != nullptr) {
            _mRootPtr->link.parent = nullptr;
        }
    }

    typename BinaryTree::IteratorType _create_iterator(typename _Node::LinkType *link, TraversalType itType) const {
        typename BinaryTree::IteratorType::NextFunc nextFunc = nullptr;
        switch (itType) {
            case TraversalType::PreOrder:
                nextFunc = next_preorder;
                break;
            case TraversalType::InOrder:
                nextFunc = next_inorder;
                break;
            case TraversalType::PostOrder:
                nextFunc = next_postorder;
                break;
            default:
                nextFunc = next_preorder;
                break;
        }
        return typename BinaryTree::IteratorType(link, nextFunc);
    }

};

} // namespace tree
} // namespace dstruct

#endif