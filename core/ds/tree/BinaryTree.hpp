// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef BINARY_TREE_HPP_DSTRUCT
#define BINARY_TREE_HPP_DSTRUCT

#include <core/common.hpp>
#include <core/ds/tree/EmbeddedBinaryTree.hpp>

namespace dstruct {
namespace tree {

template <typename T>
class BinaryTreeIterator_ : public DStructIteratorTypeSpec<T> {
    friend class BinaryTreeIterator_<const T>; // for const-it -> it
private:
    using Self = BinaryTreeIterator_;
protected:
    using Node_ = EmbeddedBinaryTreeNode<T>;
public:
    using NextFunc = typename Node_::LinkType * (*)(typename Node_::LinkType *);

public: // bigfive
    BinaryTreeIterator_(typename Node_::LinkType * link, NextFunc nextFunc) :
        mTreeNodeLinkPtr_d { link }, mNextFunc_d { nextFunc } { _sync(); }

    BinaryTreeIterator_(const BinaryTreeIterator_ &it) { *this = it; }
    BinaryTreeIterator_ & operator=(const BinaryTreeIterator_ &it) {
        mTreeNodeLinkPtr_d = it.mTreeNodeLinkPtr_d;
        mNextFunc_d = it.mNextFunc_d;
        _sync();
        return *this;
    }

    BinaryTreeIterator_(BinaryTreeIterator_ &&it) { *this = dstruct::move(it); }
    BinaryTreeIterator_ & operator=(BinaryTreeIterator_ &&it) {
        mTreeNodeLinkPtr_d = it.mTreeNodeLinkPtr_d;
        mNextFunc_d = it.mNextFunc_d;

        // reset
        it.mTreeNodeLinkPtr_d = nullptr;
        it.mNextFunc_d = nullptr;
 
        _sync();

        return *this;
    }

    ~BinaryTreeIterator_() {
        mTreeNodeLinkPtr_d = nullptr;
        mNextFunc_d = nullptr;
    }

    // from it convert to const-it
    BinaryTreeIterator_(
        const BinaryTreeIterator_<typename RemoveConst<T>::Type> &obj,
        bool __unused
    ) : BinaryTreeIterator_(obj.mTreeNodeLinkPtr_d, obj.mNextFunc_d) {
        //_sync();
    }

public: // ForwardIterator
    Self& operator++() {
        mTreeNodeLinkPtr_d = mNextFunc_d(mTreeNodeLinkPtr_d);
        _sync();
        return *this;
    }

    Self operator++(int) {
        Self old = *this;
        mTreeNodeLinkPtr_d = mNextFunc_d(mTreeNodeLinkPtr_d);
        _sync();
        return old;
    }

public:
    typename Node_::LinkType * _get_link_pointer() {
        return mTreeNodeLinkPtr_d;
    }

private:
    void _sync() {
        Self::mPointer_d = &(Node_::to_node(mTreeNodeLinkPtr_d)->data);
    }

protected:
    typename Node_::LinkType *mTreeNodeLinkPtr_d;
    NextFunc mNextFunc_d;
};

template <typename T, typename Alloc>
class BinaryTree : public DStructTypeSpec_<T, Alloc, BinaryTreeIterator_> {

public: // type
    enum TraversalType : uint8_t {
        PreOrder,
        InOrder,
        PostOrder,
    };

protected:
    using Node_      = EmbeddedBinaryTreeNode<T>;
    using AllocNode_ = AllocSpec<Node_, Alloc>;

public:
    BinaryTree(Node_ *root, size_t size) : mSize_d { size }, mRootPtr_d { root } { }
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

    Node_ *_get_root_ptr() {
        return mRootPtr_d;
    }

public: // algo
    template <typename Callback>
    void traversal(Callback cb, TraversalType ttype = TraversalType::InOrder) const {

        auto cbWrapper = [&](typename Node_::LinkType *link) {
            typename  BinaryTree::ConstIteratorType::ValueType &data = Node_::to_node(link)->data;
            cb(data);
        };

        switch (ttype) {
            case TraversalType::PreOrder:
                return  tree::preorder_traversal(first_node(Node_::to_link(mRootPtr_d), ttype), cbWrapper);
            case TraversalType::InOrder:
                return  tree::inorder_traversal(first_node(Node_::to_link(mRootPtr_d), ttype), cbWrapper);
            case TraversalType::PostOrder:
                return  tree::postorder_traversal(first_node(Node_::to_link(mRootPtr_d), ttype), cbWrapper);
            default: {
                DSTRUCT_ASSERT(false);
            }
        }

        return  tree::preorder_traversal(first_node(Node_::to_link(mRootPtr_d), ttype), cbWrapper);
    }

public: // range-for and iterator

    typename BinaryTree::IteratorType begin(TraversalType ttype = TraversalType::InOrder) const {
        return _create_iterator(first_node(Node_::to_link(mRootPtr_d), ttype), ttype);
    }

    typename BinaryTree::IteratorType end(TraversalType ttype = TraversalType::InOrder) const {
        return _create_iterator(nullptr, ttype);
    }

public:
    static Node_ * copy(Node_ *root) {
        if (!root)
            return nullptr;

        Node_ *newRoot = AllocNode_::allocate();
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

    static void clear(Node_ * &root) {
        if (root) {
            tree::postorder_traversal(&(root->link), [](typename Node_::LinkType *linkPtr) {
                Node_ *nPtr = Node_::to_node(linkPtr);
                dstruct::destroy(nPtr);
                AllocNode_::deallocate(nPtr);
            });
        }
        root = nullptr;
    }

    static typename Node_::LinkType * first_node(typename Node_::LinkType *root, TraversalType ttype = TraversalType::InOrder) {
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
    Node_ *mRootPtr_d;

    void _update_root(typename Node_::LinkType *root) {
        mRootPtr_d = Node_::to_node(root);
        if (mRootPtr_d != nullptr) {
            mRootPtr_d->link.parent = nullptr;
        }
    }

    typename BinaryTree::IteratorType _create_iterator(typename Node_::LinkType *link, TraversalType itType) const {
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