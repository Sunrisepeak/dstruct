// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef BINARY_TREE_HPP__DSTRUCT
#define BINARY_TREE_HPP__DSTRUCT

#include <core/common.hpp>
#include <core/ds/tree/EmbeddedBinaryTree.hpp>

namespace dstruct {
namespace tree {

template <typename T>
class _BinaryTreeIterator : public DStructIteratorTypeSpec<T> {
    friend class _BinaryTreeIterator<const T>; // for const-it -> it
private:
    using Self = _BinaryTreeIterator;
protected:
    using _Node = EmbeddedBinaryTreeNode<T>;
public:
    using NextFunc = typename _Node::LinkType * (*)(typename _Node::LinkType *);

public: // bigfive
    _BinaryTreeIterator(typename _Node::LinkType * link, NextFunc nextFunc) :
        mTreeNodeLinkPtr_d { link }, mNextFunc_d { nextFunc } { __sync(); }

    _BinaryTreeIterator(const _BinaryTreeIterator &it) { *this = it; }
    _BinaryTreeIterator & operator=(const _BinaryTreeIterator &it) {
        mTreeNodeLinkPtr_d = it.mTreeNodeLinkPtr_d;
        mNextFunc_d = it.mNextFunc_d;
        __sync();
        return *this;
    }

    _BinaryTreeIterator(_BinaryTreeIterator &&it) { *this = dstruct::move(it); }
    _BinaryTreeIterator & operator=(_BinaryTreeIterator &&it) {
        mTreeNodeLinkPtr_d = it.mTreeNodeLinkPtr_d;
        mNextFunc_d = it.mNextFunc_d;

        // reset
        it.mTreeNodeLinkPtr_d = nullptr;
        it.mNextFunc_d = nullptr;
 
        __sync();

        return *this;
    }

    ~_BinaryTreeIterator() {
        mTreeNodeLinkPtr_d = nullptr;
        mNextFunc_d = nullptr;
    }

    // from it convert to const-it
    _BinaryTreeIterator(
        const _BinaryTreeIterator<typename RemoveConst<T>::Type> &obj,
        bool __unused
    ) : _BinaryTreeIterator(obj.mTreeNodeLinkPtr_d, obj.mNextFunc_d) {
        //__sync();
    }

public: // ForwardIterator
    Self& operator++() {
        mTreeNodeLinkPtr_d = mNextFunc_d(mTreeNodeLinkPtr_d);
        __sync();
        return *this;
    }

    Self operator++(int) {
        Self old = *this;
        mTreeNodeLinkPtr_d = mNextFunc_d(mTreeNodeLinkPtr_d);
        __sync();
        return old;
    }

public:
    typename _Node::LinkType * __get_link_pointer() {
        return mTreeNodeLinkPtr_d;
    }

private:
    void __sync() {
        Self::mPointer_d = &(_Node::to_node(mTreeNodeLinkPtr_d)->data);
    }

protected:
    typename _Node::LinkType *mTreeNodeLinkPtr_d;
    NextFunc mNextFunc_d;
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
    BinaryTree(_Node *root, size_t size) : mSize_d { size }, mRootPtr_d { root } { }
/*
    DSTRUCT_COPY_SEMANTICS
    DSTRUCT_MOVE_SEMANTICS
    (Note: impl by subclass)
*/
    virtual ~BinaryTree() {
        clear(mRootPtr_d);
        mSize_d = 0;
    }

public:
    bool empty() const {
        return mSize_d == 0;
    }

    typename BinaryTree::SizeType
    size() const {
        return mSize_d;
    }

    typename BinaryTree::SizeType
    capacity() const {
        return mSize_d;
    }

    void clear() {
        clear(mRootPtr_d);
        mSize_d = 0;
    }

    _Node *_get_root_ptr() {
        return mRootPtr_d;
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
                return  tree::preorder_traversal(first_node(_Node::to_link(mRootPtr_d), ttype), cbWrapper);
            case TraversalType::InOrder:
                return  tree::inorder_traversal(first_node(_Node::to_link(mRootPtr_d), ttype), cbWrapper);
            case TraversalType::PostOrder:
                return  tree::postorder_traversal(first_node(_Node::to_link(mRootPtr_d), ttype), cbWrapper);
            default: {
                DSTRUCT_ASSERT(false);
            }
        }

        return  tree::preorder_traversal(first_node(_Node::to_link(mRootPtr_d), ttype), cbWrapper);
    }

public: // range-for and iterator

    typename BinaryTree::IteratorType begin(TraversalType ttype = TraversalType::InOrder) const {
        return _create_iterator(first_node(_Node::to_link(mRootPtr_d), ttype), ttype);
    }

    typename BinaryTree::IteratorType end(TraversalType ttype = TraversalType::InOrder) const {
        return _create_iterator(nullptr, ttype);
    }

public:
    static _Node * copy(_Node *root) {
        if (!root)
            return nullptr;

        _Node *newRoot = _AllocNode::allocate();
        newRoot->data = root->data;
        newRoot->parent = nullptr;

        newRoot->left = copy(root->left);
        if (newRoot->left)
            newRoot->left->parent = newRoot;

        newRoot->right = copy(root->right);
        if (newRoot->right)
            newRoot->right->parent = newRoot;

        return newRoot;
    }

    static void clear(_Node * &root) {
        if (root) {
            tree::postorder_traversal(&(root->link), [](typename _Node::LinkType *linkPtr) {
                _Node *nPtr = _Node::to_node(linkPtr);
                dstruct::destroy(nPtr);
                _AllocNode::deallocate(nPtr);
            });
        }
        root = nullptr;
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
    size_t mSize_d;
    _Node *mRootPtr_d;

    void _update_root(typename _Node::LinkType *root) {
        mRootPtr_d = _Node::to_node(root);
        if (mRootPtr_d != nullptr) {
            mRootPtr_d->link.parent = nullptr;
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