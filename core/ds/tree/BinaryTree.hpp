#ifndef __BINARY_TREE_HPP__DSTRUCT
#define __BINARY_TREE_HPP__DSTRUCT

#include <core/ds/tree/Tree.hpp>

namespace dstruct {

namespace tree {

/*
   Root
   / \
  L   R
*/
struct _BinaryTreeLink {
    _BinaryTreeLink *left;
    _BinaryTreeLink *right;

    static _BinaryTreeLink * left_rotate(_BinaryTreeLink *root) {
        _BinaryTreeLink *newRoot = root->right;
        if (nullptr != newRoot->left) {
            root->right = newRoot->left;
        }
        newRoot->left = root;
        return newRoot;
    }

    static _BinaryTreeLink * right_rotate(_BinaryTreeLink *root) {
        _BinaryTreeLink *newRoot = root->left;
        if (nullptr != newRoot->right) {
            root->left = newRoot->right;
        }
        newRoot->right = root;
        return newRoot;
    }

    static int height(_BinaryTreeLink *root) {
        int h = 0;
        if (root) {
            int lH, rH;
            lH = height(root->left);
            rH = height(root->right);
            h = (lH > rH ? lH : rH) + 1;
        }
        return h;
    }

    template <typename Callback>
    static void preorder_traversal(_BinaryTreeLink *root, Callback &cb) {
        if (root) {
            cb(root);
            preorder_traversal(root->left, cb);
            preorder_traversal(root->right, cb);
        }
    }

    template <typename Callback>
    static void inorder_traversal(_BinaryTreeLink *root, Callback &cb) {
        if (root) {
            inorder_traversal(root->left, cb);
            cb(root);
            inorder_traversal(root->right, cb);
        }
    }

    template <typename Callback>
    static void postorder_traversal(_BinaryTreeLink *root, Callback &cb) {
        if (root) {
            postorder_traversal(root->left, cb);
            postorder_traversal(root->right, cb);
            cb(root);
        }
    }
};

template<typename T>
using BinaryTreeNode = TreeNode<T, _BinaryTreeLink>;

} // namespace tree

} // namespace dstruct


#endif