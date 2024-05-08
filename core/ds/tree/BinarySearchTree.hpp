// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef BINARY_SEARCH_TREE_HPP_DSTRUCT
#define BINARY_SEARCH_TREE_HPP_DSTRUCT

#include <core/common.hpp>
#include <core/ds/tree/EmbeddedBinaryTree.hpp>
#include <core/ds/tree/BinaryTree.hpp>
#include <core/ds/tree/BinarySearchTreeBase.hpp>

namespace dstruct {
namespace tree {

// TODO: https://en.cppreference.com/w/cpp/language/operator_comparison
template <typename T, typename CMP, typename Alloc>
class BinarySearchTree : public BinaryTree<T, Alloc> {
    using BinaryTree_e  = BinaryTree<T, Alloc>;
public:
    using TraversalType = typename BinaryTree_e::TraversalType;
protected:
    using Node_         = typename BinaryTree_e::Node_;
    using AllocNode_    = typename BinaryTree_e::AllocNode_;

public: // big five

    BinarySearchTree(CMP cmp = CMP()) : BinaryTree_e { nullptr, 0 }, mCmp_d { cmp } { }

    BinarySearchTree(const PrimitiveIterator<T> &begin, const PrimitiveIterator<T> &end, CMP cmp = CMP()) :
        BinarySearchTree(cmp) {
        for (auto it = begin; it != end; it++) {
            push(*it);
        }
    }

    DSTRUCT_COPY_SEMANTICS(BinarySearchTree) {
        BinaryTree_e::clear();
        BinaryTree_e::mRootPtr_d = BinaryTree_e::copy(ds.mRootPtr_d);
        BinaryTree_e::mSize_d = ds.mSize_d;
        mCmp_d = ds.mCmp_d;
        return *this;
    }

    DSTRUCT_MOVE_SEMANTICS(BinarySearchTree) {
        BinaryTree_e::clear();

        // move
        BinaryTree_e::mRootPtr_d = ds.mRootPtr_d;
        BinaryTree_e::mSize_d = ds.mSize_d;
        mCmp_d = ds.mCmp_d;

        // reset
        ds.mRootPtr_d = nullptr;
        ds.mSize_d = 0;

        return *this;
    }

    ~BinarySearchTree() = default;

public: // push/pop

    void push(const T &obj) {
        auto tree = _insert(obj);
        if (BinaryTree_e::mRootPtr_d == nullptr)
            BinaryTree_e::_update_root(tree);
        BinaryTree_e::mSize_d++;
    }

    void pop(const T &obj) {
        if (BinaryTree_e::mSize_d == 0) return; // TODO: better method?
        auto root = _delete(Node_::to_link(BinaryTree_e::mRootPtr_d), obj);
        if (BinaryTree_e::mRootPtr_d != Node_::to_node(root)) {
            BinaryTree_e::_update_root(root);
        }
        // mSize_d--; in _try_to_delete
    }

public:
    typename BinarySearchTree::ConstIteratorType
    find(const T &obj) const {
        auto target = BinarySearchTreeBase<T, CMP>::_find(Node_::to_link(BinaryTree_e::mRootPtr_d), obj, mCmp_d);
        return typename BinarySearchTree::ConstIteratorType(
            BinaryTree_e::_create_iterator(target, TraversalType::InOrder),
            true
        );
    }

    typename BinarySearchTree::ConstIteratorType
    erase(typename BinarySearchTree::ConstIteratorType it) {

        auto target = Node_::data_to_link(it.operator->());
        auto parent = target->parent;
        auto next = it; it++; // get next node

        auto tree = _try_to_delete(target);

        if (target != tree) { // deleted directly
            next = it;

            if (parent == nullptr) { // is root
                BinaryTree_e::_update_root(tree);
                return next;
            }

            if (parent->left == target) {
                parent->left = tree;
            } else {
                parent->right = tree;
            }
        }

        return next;
    }

public: // range-for and iterator

    typename BinarySearchTree::ConstIteratorType
    begin(TraversalType ttype = TraversalType::InOrder) const {
        return typename BinaryTree_e::ConstIteratorType(
            BinaryTree_e::_create_iterator(
                BinaryTree_e::first_node(Node_::to_link(BinaryTree_e::mRootPtr_d), ttype),
                ttype
            ),
            true
        );
    }

    typename BinarySearchTree::ConstIteratorType
    end(TraversalType ttype = TraversalType::InOrder) const {
        return typename BinaryTree_e::ConstIteratorType(
            BinaryTree_e::_create_iterator(nullptr, ttype),
            true
        );
    }

protected:
    CMP mCmp_d;

    typename Node_::LinkType * _insert(const T &obj) {

        typename Node_::LinkType * target = Node_::to_link(BinaryTree_e::mRootPtr_d);
        typename Node_::LinkType * parent = nullptr;

        // find parent(leaf-node)
        while (target != nullptr) {
            parent = target;
            if (mCmp_d(obj, Node_::to_node(target)->data)) {
                target = target->left;
            } else if (mCmp_d(Node_::to_node(target)->data, obj)) {
                target = target->right;
            } else {
                // TODO: pls check your cmp, a < b, b < a, but a != b
                // TODO: to support dup-data
                return target;
            }
        }

        // create node
        auto nodePtr = AllocNode_::allocate();
        auto newNodeLink = Node_::to_link(nodePtr);
        dstruct::construct(nodePtr, Node_(obj));

        if (parent != nullptr) { // nodePtr isn't root
            newNodeLink->parent = parent;
            decltype(parent) subTree = nullptr; // for readability
            if (mCmp_d(obj, Node_::to_node(parent)->data)) {
                subTree = parent->left;
                parent->left = newNodeLink;
                newNodeLink->left = subTree;
            } else {
                subTree = parent->right;
                parent->right = newNodeLink;
                newNodeLink->right = subTree;
            }

            if (subTree) subTree->parent = newNodeLink;
        }

        return newNodeLink;
    }

    typename Node_::LinkType * _delete(typename Node_::LinkType *root, const T &obj) {
        auto nPtr = Node_::to_node(root);
        if (mCmp_d(obj, nPtr->data)) {
            root->left = _delete(root->left, obj);
        } else if (mCmp_d(nPtr->data, obj)) {
            root->right = _delete(root->right, obj);
        } else {
            // try to del the node
            return _try_to_delete(root);
        }
        return root;
    }

    typename Node_::LinkType * _try_to_delete(typename Node_::LinkType *linkPtr) {
        typename Node_::LinkType *subTree = nullptr;

        if (nullptr == linkPtr->left) {
            subTree = linkPtr->right;
        } else if (nullptr == linkPtr->right) {
            subTree = linkPtr->left;
        } else { // have l and r, haven't del the node directly
            // step1: find right-bot
            typename Node_::LinkType *tmp = linkPtr->right;
            while (nullptr == tmp->left) {
                tmp = tmp->left;
            }

            // step2: move val, and del obj from nPtr->data change to tmpPtr->data
            Node_ *nPtr = Node_::to_node(linkPtr);
            Node_ *tmpPtr = Node_::to_node(tmp);

            nPtr->data = tmpPtr->data;
            linkPtr->right = _delete(linkPtr->right, tmpPtr->data); // retry

            // Note: return
            return linkPtr;
        }

        if (subTree)
            subTree->parent = linkPtr->parent;

        // real delete
        auto nPtr = Node_::to_node(linkPtr);
        dstruct::destroy(nPtr);
        AllocNode_::deallocate(nPtr);
        BinaryTree_e::mSize_d--;

        return subTree;
    }
};

} // tree
} // dstruct

#endif