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
#include <core/ds/tree/EmbeddedBinaryTree.hpp>
#include <core/ds/tree/BinaryTree.hpp>

namespace dstruct {

template <typename T, typename CMP, typename Alloc>
class AVLTree {

private:
    struct __Data {
        int height;
        T val;
    };
protected:
    using _Node      = tree::EmbeddedBinaryTreeNode<__Data>; 
    using _AllocNode = AllocSpec<_Node, Alloc>;

public:
    AVLTree(CMP cmp = CMP()) : _mSize { 0 }, _mCmp { cmp }, _mRootPtr { nullptr } { }

public:
    void insert(const T &element) {
        auto root = _insert(_Node::to_link(_mRootPtr), element);
        _update_root(root);
        _mSize++;
    }

    void erase() {

    }

    int height() const {
        return _mRootPtr ? _mRootPtr->data.height : 0;
    }

protected:
    size_t _mSize;
    CMP _mCmp;
    _Node *_mRootPtr;

    void _update_root(typename _Node::LinkType *root) {
        _mRootPtr = _Node::to_node(root);
        if (_mRootPtr != nullptr) {
            _mRootPtr->link.parent = nullptr;
        }
    }

    typename _Node::LinkType * _insert(typename _Node::LinkType *root, const T &element) {
        _Node *rootNode = nullptr;
        if (root == nullptr) { // create node
            rootNode = _AllocNode::allocate();
            dstruct::construct(rootNode, _Node(__Data{0, element}));
            root = _Node::to_link(rootNode);
        } else {
            rootNode = _Node::to_node(root);
            if (_mCmp(element, rootNode->data.val)) {
                root->left = _insert(root->left, element);
                if (_height(root->left) - _height(root->right) == 2) {
                    if (_mCmp(_Node::to_node(root->left)->data.val, element)) { // LR: double-rotate
                        root = _left_rotate(root->left);
                    }
                    root = _right_rotate(root);
                }
            } else if (_mCmp(rootNode->data.val, element)) {
                root->right = _insert(root->right, element);
                if (_height(root->right) - _height(root->left) == 2) {
                    if (_mCmp(element, _Node::to_node(root->right)->data.val)) { // RL: double-rotate
                        root = _right_rotate(root->right); 
                    }
                    root = _left_rotate(root);
                }
            } else {
                DSTRUCT_ASSERT(false); // CMP-Failed
            }
        }

        rootNode = _Node::to_node(root); // update, if root changed
        rootNode->data.height = dstruct::max(_height(rootNode->link.left), _height(rootNode->link.right)) + 1;
        printf("_insert: %d %d: L-H %d, R-H %d\n",
            rootNode->data.height, rootNode->data.val, _height(rootNode->link.left), _height(rootNode->link.right));

        return root;
    }

    int _height(typename _Node::LinkType *root) {
        if (root == nullptr)
            return 0;
        return _Node::to_node(root)->data.height;
    }

    typename _Node::LinkType * _left_rotate(typename _Node::LinkType *root) {
        root = tree::left_rotate(root);
        auto rootNode = _Node::to_node(root);
        auto leftNode = _Node::to_node(root->left);
        printf("_left_rotate: root: %d %d left node: %d %d\n",
            rootNode->data.height, rootNode->data.val, leftNode->data.height, leftNode->data.val);
        leftNode->data.height = dstruct::max(_height(leftNode->link.left), _height(leftNode->link.left)) + 1;
        rootNode->data.height = dstruct::max(leftNode->data.height, _height(rootNode->link.left)) + 1;
        printf("_left_rotate: root: %d %d left node: %d %d\n",
            rootNode->data.height, rootNode->data.val, leftNode->data.height, leftNode->data.val);
        return root;
    }

    typename _Node::LinkType * _right_rotate(typename _Node::LinkType *root) {
        root = tree::right_rotate(root);
        auto rootNode = _Node::to_node(root);
        auto rightNode = _Node::to_node(root->right);
        printf("_right_rotate: root: %d %d right node: %d %d\n",
            rootNode->data.height, rootNode->data.val, rightNode->data.height, rightNode->data.val);
        rightNode->data.height = dstruct::max(_height(rightNode->link.left), _height(rightNode->link.left)) + 1;
        rootNode->data.height = dstruct::max(_height(rootNode->link.left), rightNode->data.height) + 1;
        printf("_right_rotate: root: %d %d right node: %d %d\n",
            rootNode->data.height, rootNode->data.val, rightNode->data.height, rightNode->data.val);
        return root;
    }
};

}

#endif