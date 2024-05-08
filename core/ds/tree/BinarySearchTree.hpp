// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef BINARY_SEARCH_TREE_HPP__DSTRUCT
#define BINARY_SEARCH_TREE_HPP__DSTRUCT

#include <core/common.hpp>
#include <core/ds/tree/EmbeddedBinaryTree.hpp>
#include <core/ds/tree/BinaryTree.hpp>
#include <core/ds/tree/BinarySearchTreeBase.hpp>

namespace dstruct {
namespace tree {

// TODO: https://en.cppreference.com/w/cpp/language/operator_comparison
template <typename T, typename CMP, typename Alloc>
class BinarySearchTree : public BinaryTree<T, Alloc> {
    using __BinaryTree  = BinaryTree<T, Alloc>;
public:
    using TraversalType = typename __BinaryTree::TraversalType;
protected:
    using _Node         = typename __BinaryTree::_Node;
    using _AllocNode    = typename __BinaryTree::_AllocNode;

public: // big five

    BinarySearchTree(CMP cmp = CMP()) : __BinaryTree { nullptr, 0 }, _mCmp { cmp } { }

    BinarySearchTree(const PrimitiveIterator<T> &begin, const PrimitiveIterator<T> &end, CMP cmp = CMP()) :
        BinarySearchTree(cmp) {
        for (auto it = begin; it != end; it++) {
            push(*it);
        }
    }

    DSTRUCT_COPY_SEMANTICS(BinarySearchTree) {
        __BinaryTree::clear();
        __BinaryTree::_mRootPtr = __BinaryTree::copy(ds._mRootPtr);
        __BinaryTree::_mSize = ds._mSize;
        _mCmp = ds._mCmp;
        return *this;
    }

    DSTRUCT_MOVE_SEMANTICS(BinarySearchTree) {
        __BinaryTree::clear();

        // move
        __BinaryTree::_mRootPtr = ds._mRootPtr;
        __BinaryTree::_mSize = ds._mSize;
        _mCmp = ds._mCmp;

        // reset
        ds._mRootPtr = nullptr;
        ds._mSize = 0;

        return *this;
    }

    ~BinarySearchTree() = default;

public: // push/pop

    void push(const T &obj) {
        auto tree = _insert(obj);
        if (__BinaryTree::_mRootPtr == nullptr)
            __BinaryTree::_update_root(tree);
        __BinaryTree::_mSize++;
    }

    void pop(const T &obj) {
        if (__BinaryTree::_mSize == 0) return; // TODO: better method?
        auto root = _delete(_Node::to_link(__BinaryTree::_mRootPtr), obj);
        if (__BinaryTree::_mRootPtr != _Node::to_node(root)) {
            __BinaryTree::_update_root(root);
        }
        // _mSize--; in _try_to_delete
    }

public:
    typename BinarySearchTree::ConstIteratorType
    find(const T &obj) const {
        auto target = BinarySearchTreeBase<T, CMP>::_find(_Node::to_link(__BinaryTree::_mRootPtr), obj, _mCmp);
        return typename BinarySearchTree::ConstIteratorType(
            __BinaryTree::_create_iterator(target, TraversalType::InOrder),
            true
        );
    }

    typename BinarySearchTree::ConstIteratorType
    erase(typename BinarySearchTree::ConstIteratorType it) {

        auto target = _Node::data_to_link(it.operator->());
        auto parent = target->parent;
        auto next = it; it++; // get next node

        auto tree = _try_to_delete(target);

        if (target != tree) { // deleted directly
            next = it;

            if (parent == nullptr) { // is root
                __BinaryTree::_update_root(tree);
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
        return typename __BinaryTree::ConstIteratorType(
            __BinaryTree::_create_iterator(
                __BinaryTree::first_node(_Node::to_link(__BinaryTree::_mRootPtr), ttype),
                ttype
            ),
            true
        );
    }

    typename BinarySearchTree::ConstIteratorType
    end(TraversalType ttype = TraversalType::InOrder) const {
        return typename __BinaryTree::ConstIteratorType(
            __BinaryTree::_create_iterator(nullptr, ttype),
            true
        );
    }

protected:
    CMP _mCmp;

    typename _Node::LinkType * _insert(const T &obj) {

        typename _Node::LinkType * target = _Node::to_link(__BinaryTree::_mRootPtr);
        typename _Node::LinkType * parent = nullptr;

        // find parent(leaf-node)
        while (target != nullptr) {
            parent = target;
            if (_mCmp(obj, _Node::to_node(target)->data)) {
                target = target->left;
            } else if (_mCmp(_Node::to_node(target)->data, obj)) {
                target = target->right;
            } else {
                // TODO: pls check your cmp, a < b, b < a, but a != b
                // TODO: to support dup-data
                return target;
            }
        }

        // create node
        auto nodePtr = _AllocNode::allocate();
        auto newNodeLink = _Node::to_link(nodePtr);
        dstruct::construct(nodePtr, _Node(obj));

        if (parent != nullptr) { // nodePtr isn't root
            newNodeLink->parent = parent;
            decltype(parent) subTree = nullptr; // for readability
            if (_mCmp(obj, _Node::to_node(parent)->data)) {
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

    typename _Node::LinkType * _delete(typename _Node::LinkType *root, const T &obj) {
        auto nPtr = _Node::to_node(root);
        if (_mCmp(obj, nPtr->data)) {
            root->left = _delete(root->left, obj);
        } else if (_mCmp(nPtr->data, obj)) {
            root->right = _delete(root->right, obj);
        } else {
            // try to del the node
            return _try_to_delete(root);
        }
        return root;
    }

    typename _Node::LinkType * _try_to_delete(typename _Node::LinkType *linkPtr) {
        typename _Node::LinkType *subTree = nullptr;

        if (nullptr == linkPtr->left) {
            subTree = linkPtr->right;
        } else if (nullptr == linkPtr->right) {
            subTree = linkPtr->left;
        } else { // have l and r, haven't del the node directly
            // step1: find right-bot
            typename _Node::LinkType *tmp = linkPtr->right;
            while (nullptr == tmp->left) {
                tmp = tmp->left;
            }

            // step2: move val, and del obj from nPtr->data change to tmpPtr->data
            _Node *nPtr = _Node::to_node(linkPtr);
            _Node *tmpPtr = _Node::to_node(tmp);

            nPtr->data = tmpPtr->data;
            linkPtr->right = _delete(linkPtr->right, tmpPtr->data); // retry

            // Note: return
            return linkPtr;
        }

        if (subTree)
            subTree->parent = linkPtr->parent;

        // real delete
        auto nPtr = _Node::to_node(linkPtr);
        dstruct::destroy(nPtr);
        _AllocNode::deallocate(nPtr);
        __BinaryTree::_mSize--;

        return subTree;
    }
};

} // tree
} // dstruct

#endif