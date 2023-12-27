// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef __AVL_TREE_HPP__DSTRUCT
#define __AVL_TREE_HPP__DSTRUCT

#include <core/common.hpp>
#include <core/ds/tree/BinaryTree.hpp>

namespace dstruct {

template <typename T>
struct _AVLData {
    int height;
    T val;

    _AVLData(const T &_val) : height { 0 }, val { _val } { }

};

// this is a wrap for CMP of T-Type
template <typename T, typename CMP>
struct _AVLDataCMP {
    _AVLDataCMP(CMP cmp = CMP()) : __mCMP { cmp } {}

    bool operator()(const _AVLData<T> &a, const _AVLData<T> &b) const {
        return __mCMP(a.val, b.val);
    }

private:
    CMP __mCMP;
};

template <typename T>
class _AVLTreeIterator :  public DStructIteratorTypeSpec<const T /* only-read */> {
private:
    using __Self = _AVLTreeIterator;
protected:
    using _Node = tree::EmbeddedBinaryTreeNode<T>;
    using _BinaryTreeConstIterator = tree::_BinaryTreeIterator<const _AVLData<T>>;
public:
    _AVLTreeIterator(const _BinaryTreeConstIterator &it) : _mIterator { it } {
        __sync();
    }

public: // ForwardIterator
    __Self& operator++() {
        _mIterator++;
        __sync();
        return *this;
    }

    __Self operator++(int) {
        __Self old = *this;
        _mIterator++;
        __sync();
        return old;
    }

private:
    void __sync() {
        if (nullptr == _mIterator.operator->())
            __Self::_mPointer = nullptr;
        else
            __Self::_mPointer = &(_mIterator->val);
    }

protected:
    _BinaryTreeConstIterator _mIterator;
};

template <typename T, typename CMP, typename Alloc>
class AVLTree : public tree::BinaryTree<_AVLData<T>, Alloc> {

    using __BinaryTree  = tree::BinaryTree<_AVLData<T>, Alloc>;
public:
    using ConstIteratorType = _AVLTreeIterator<T>;
    using TraversalType = typename __BinaryTree::TraversalType;
protected:
    using _Node         = typename __BinaryTree::_Node;
    using _AllocNode    = typename __BinaryTree::_AllocNode;

public:
    AVLTree(CMP cmp = CMP()) : __BinaryTree { nullptr, 0 }, _mCmp { cmp } { }

public:
    void push(const T &element) {
        auto root = _insert(_Node::to_link(__BinaryTree::_mRootPtr), element);
        __BinaryTree::_update_root(root);
        __BinaryTree::_mSize++;
    }

    void pop(const T &obj) {
        if (__BinaryTree::_mSize == 0) return; // TODO: better method?
        auto root = _delete(_Node::to_link(__BinaryTree::_mRootPtr), obj);
        if (__BinaryTree::_mRootPtr != _Node::to_node(root)) {
            __BinaryTree::_update_root(root);
        }
    }

    typename AVLTree::ConstIteratorType
    find(const T &obj) const {
        using CMPWrapper = _AVLDataCMP<T, CMP>; // TODO: optimize find(delete CMPWrapper?)
        auto target = BinarySearchTreeBase<_AVLData<T>, CMPWrapper>::_find(
            _Node::to_link(__BinaryTree::_mRootPtr),
            obj,
            CMPWrapper(_mCmp)
        );
        return typename __BinaryTree::ConstIteratorType(
            __BinaryTree::_create_iterator(target, TraversalType::InOrder),
            true
        );
    }

    int height() const {
        return __BinaryTree::_mRootPtr ? __BinaryTree::_mRootPtr->data.height : 0;
    }

public: // range-for and iterator

    typename AVLTree::ConstIteratorType
    begin(TraversalType ttype = TraversalType::InOrder) const {
        return __BinaryTree::begin();
    }

    typename AVLTree::ConstIteratorType
    end(TraversalType ttype = TraversalType::InOrder) const {
        return __BinaryTree::end();
    }

protected:
    CMP _mCmp;

    int _height(typename _Node::LinkType *root) {
        if (root == nullptr)
            return 0;
        return _Node::to_node(root)->data.height;
    }

    int _balance_factor(typename _Node::LinkType *root) {
        return root == nullptr ? 0 : _height(root->left) - _height(root->right);
    }

    typename _Node::LinkType * _check_and_balance(typename _Node::LinkType *root) {
        int balance = _balance_factor(root);
        if (balance > 1) { // need r-rotate
            if (_balance_factor(root->left) < 0) {
                root->left = _left_rotate(root->left);
            }
            root = _right_rotate(root);
        } else if (balance < -1) { // // need l-rotate
            if (_balance_factor(root->right) > 0) {
                root->right = _right_rotate(root->right);
            }
            root = _left_rotate(root);
        }
        return root;
    }

    typename _Node::LinkType * _insert(typename _Node::LinkType *root, const T &element) {
        _Node *rootNode = nullptr;
        if (root == nullptr) { // create node
            rootNode = _AllocNode::allocate();
            dstruct::construct(rootNode, _Node(_AVLData<T>(element)));
            root = _Node::to_link(rootNode);
        } else {
            rootNode = _Node::to_node(root);
            if (_mCmp(element, rootNode->data.val)) {
                root->left = _insert(root->left, element);
                if (_height(root->left) - _height(root->right) == 2) {
                    if (_mCmp(_Node::to_node(root->left)->data.val, element)) { // LR: double-rotate
                        root->left = _left_rotate(root->left);
                    }
                    root = _right_rotate(root);
                }
            } else if (_mCmp(rootNode->data.val, element)) {
                root->right = _insert(root->right, element);
                if (_height(root->right) - _height(root->left) == 2) {
                    if (_mCmp(element, _Node::to_node(root->right)->data.val)) { // RL: double-rotate
                        root->right = _right_rotate(root->right);
                        // TODO: verify need update root-height?
                    }
                    root = _left_rotate(root);
                }
            } else {
                DSTRUCT_ASSERT(false); // CMP-Failed
            }
        }

        rootNode = _Node::to_node(root); // update, if root changed
        rootNode->data.height = dstruct::max(_height(rootNode->link.left), _height(rootNode->link.right)) + 1;

        return root;
    }

    typename _Node::LinkType * _delete(typename _Node::LinkType *root, const T &obj) {
        auto nPtr = _Node::to_node(root);
        if (_mCmp(obj, nPtr->data.val)) {
            root->left = _delete(root->left, obj);
        } else if (_mCmp(nPtr->data.val, obj)) {
            root->right = _delete(root->right, obj);
        } else {

            // l and r isn't nullptr, need to find leaf-node(r-side)
            if (nullptr != root->left && nullptr != root->right) {
                // step1: find right-bot
                typename _Node::LinkType *tmp = root->right;
                while (nullptr == tmp->left) {
                    tmp = tmp->left;
                }
                // step2: move val, and del obj from nPtr->data change to tmpPtr->data
                _Node *tmpPtr = _Node::to_node(tmp);
                nPtr->data = tmpPtr->data;
                root->right = _delete(root->right, tmpPtr->data.val); // retry
            } else {
                typename _Node::LinkType *subTree = nullptr;

                if (nullptr == root->left) {
                    subTree = root->right;
                } else if (nullptr == root->right) {
                    subTree = root->left;
                }

                if (subTree) subTree->parent = root->parent;

                // real delete
                dstruct::destroy(nPtr);
                _AllocNode::deallocate(nPtr);
                __BinaryTree::_mSize--;
                return subTree; // Note: only need return sub-tree directly
            }
        }

        nPtr->data.height = dstruct::max(_height(nPtr->link.left), _height(nPtr->link.right)) + 1;

        return _check_and_balance(root);
    }

    typename _Node::LinkType * _left_rotate(typename _Node::LinkType *root) {
        root = tree::left_rotate(root);
        auto rootNode = _Node::to_node(root);
        auto leftNode = _Node::to_node(root->left);
        leftNode->data.height = dstruct::max(_height(leftNode->link.left), _height(leftNode->link.right)) + 1;
        rootNode->data.height = dstruct::max(leftNode->data.height, _height(rootNode->link.left)) + 1;
        return root;
    }

    typename _Node::LinkType * _right_rotate(typename _Node::LinkType *root) {
        root = tree::right_rotate(root);
        auto rootNode = _Node::to_node(root);
        auto rightNode = _Node::to_node(root->right);
        rightNode->data.height = dstruct::max(_height(rightNode->link.left), _height(rightNode->link.right)) + 1;
        rootNode->data.height = dstruct::max(_height(rootNode->link.left), rightNode->data.height) + 1;
        return root;
    }
};

}

#endif