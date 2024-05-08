// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef AVL_TREE_HPP_DSTRUCT
#define AVL_TREE_HPP_DSTRUCT

#include <core/common.hpp>
#include <core/ds/tree/BinaryTree.hpp>

namespace dstruct {

template <typename T>
struct AVLData_ {
    int height;
    T val;

    AVLData_() : height { 0 }, val { } { }

    AVLData_(const T &_val) : height { 0 }, val { _val } { }

};

// this is a wrap for CMP of T-Type
template <typename T, typename CMP>
struct AVLDataCMP_ {
    AVLDataCMP_(CMP cmp = CMP()) : mCMP_d_d { cmp } {}

    bool operator()(const AVLData_<T> &a, const AVLData_<T> &b) const {
        return mCMP_d_d(a.val, b.val);
    }

private:
    CMP mCMP_d_d;
};

template <typename T>
class AVLTreeIterator_ :  public DStructIteratorTypeSpec<const T> {
private:
    using Self = AVLTreeIterator_;
protected:
    using Node_ = tree::EmbeddedBinaryTreeNode<T>;
    using BinaryTreeIterator_ = tree::BinaryTreeIterator_<AVLData_<T>>;
public:
    AVLTreeIterator_(const BinaryTreeIterator_ &it) : mIterator_d { it } {
        _sync();
    }

public: // ForwardIterator
    Self& operator++() {
        mIterator_d++;
        _sync();
        return *this;
    }

    Self operator++(int) {
        Self old = *this;
        mIterator_d++;
        _sync();
        return old;
    }

public:
    typename Node_::LinkType * _get_link_pointer() {
        return mIterator_d._get_link_pointer();
    }

private:
    void _sync() {
        if (nullptr == mIterator_d.operator->())
            Self::mPointer_d = nullptr;
        else
            Self::mPointer_d = &(mIterator_d->val);
    }

protected:
    BinaryTreeIterator_ mIterator_d;
};

// TODO: support dup-data
template <typename T, typename CMP, typename Alloc>
class AVLTree : public tree::BinaryTree<AVLData_<T>, Alloc> {

    using BinaryTree_e  = tree::BinaryTree<AVLData_<T>, Alloc>;

protected:
    using Node_         = typename BinaryTree_e::Node_;
    using AllocNode_    = typename BinaryTree_e::AllocNode_;

public:
    using ValueType            = T;
    using ReferenceType        = ValueType &;
    using ConstReferenceType   = const ValueType &;
    using PointerType          = ValueType *;
    using ConstPointerType     = const ValueType *;
    using SizeType             = unsigned long long;
    using DifferenceType       = long long;

public:
    using IteratorType      = AVLTreeIterator_<T>;
    using ConstIteratorType = AVLTreeIterator_<T>;
    using TraversalType = typename BinaryTree_e::TraversalType;

public:
    AVLTree(CMP cmp = CMP()) : BinaryTree_e { nullptr, 0 }, mCmp_d { cmp } { }

public:
    void push(const T &element) {
        auto root = _insert(Node_::to_link(BinaryTree_e::mRootPtr_d), element);
        BinaryTree_e::_update_root(root);
        BinaryTree_e::mSize_d++;
    }

    void pop(const T &obj) {
        if (BinaryTree_e::mSize_d == 0) return; // TODO: better method?
        auto root = _delete(Node_::to_link(BinaryTree_e::mRootPtr_d), obj);
        if (BinaryTree_e::mRootPtr_d != Node_::to_node(root)) {
            BinaryTree_e::_update_root(root);
        }
    }

public:

    typename AVLTree::ConstIteratorType
    find(const T &obj) const {
        using CMPWrapper = AVLDataCMP_<T, CMP>; // TODO: optimize find(delete CMPWrapper?)
        auto target = BinarySearchTreeBase<AVLData_<T>, CMPWrapper>::_find(
            Node_::to_link(BinaryTree_e::mRootPtr_d),
            obj,
            CMPWrapper(mCmp_d)
        );
        return BinaryTree_e::_create_iterator(target, TraversalType::InOrder);
    }

    typename AVLTree::ConstIteratorType
    erase(typename AVLTree::ConstIteratorType it) {
        auto target = it._get_link_pointer();
        decltype(it) next = ++it;
        typename RemoveConst<T>::Type nextData;
        bool needUpdateNext = false;
        
        if (next._get_link_pointer() != nullptr) {
            nextData = *next;
            needUpdateNext = true;
        }

        _delete_by_target(target);

        return needUpdateNext ? find(nextData) : next;
    }

    int height() const {
        return BinaryTree_e::mRootPtr_d ? BinaryTree_e::mRootPtr_d->data.height : 0;
    }

public: // range-for and iterator

    typename AVLTree::ConstIteratorType
    begin(TraversalType ttype = TraversalType::InOrder) const {
        return BinaryTree_e::begin();
    }

    typename AVLTree::ConstIteratorType
    end(TraversalType ttype = TraversalType::InOrder) const {
        return BinaryTree_e::end();
    }

protected:
    CMP mCmp_d;

    typename Node_::LinkType * _check_and_balance(typename Node_::LinkType *root) {
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

    void _rebalance_after_delete(typename Node_::LinkType *root) {
        while (root != nullptr) {
            // bottom-up balance
            auto parent = root->parent;
            auto newRoot = _check_and_balance(root);

            if (parent != nullptr) {
                if (parent->left == root) {
                    parent->left = newRoot;
                } else {
                    parent->right = newRoot;
                }
                _update_height(parent);
            } else if (newRoot != root) {
                BinaryTree_e::_update_root(newRoot);
            }

            root = parent;
        }
    }

    typename Node_::LinkType *
    _insert(typename Node_::LinkType *root, const T &element, typename Node_::LinkType *parent = nullptr) {
        Node_ *rootNode = nullptr;
        if (root == nullptr) { // create node
            rootNode = AllocNode_::allocate();
            dstruct::construct(rootNode, Node_(AVLData_<T>(element)));
            root = Node_::to_link(rootNode);
            root->parent = parent;
        } else {
            rootNode = Node_::to_node(root);
            if (mCmp_d(element, rootNode->data.val)) {
                root->left = _insert(root->left, element, root);
                if (_height(root->left) - _height(root->right) == 2) {
                    if (mCmp_d(Node_::to_node(root->left)->data.val, element)) { // LR: double-rotate
                        root->left = _left_rotate(root->left);
                    }
                    root = _right_rotate(root);
                }
            } else if (mCmp_d(rootNode->data.val, element)) {
                root->right = _insert(root->right, element, root);
                if (_height(root->right) - _height(root->left) == 2) {
                    if (mCmp_d(element, Node_::to_node(root->right)->data.val)) { // RL: double-rotate
                        root->right = _right_rotate(root->right);
                        // TODO: verify need update root-height?
                    }
                    root = _left_rotate(root);
                }
            } else {
                DSTRUCT_ASSERT(false); // CMP-Failed
            }
        }

        _update_height(root); // update when root changed

        return root;
    }

    typename Node_::LinkType * _delete_by_target(typename Node_::LinkType *target) {
        if (target == nullptr) {
            return nullptr;
        }

        auto parent = target->parent;
        auto leftChild = target->left;
        auto rightChild = target->right;

        if (leftChild == nullptr && rightChild == nullptr) {
            // Case 1: Node has no children
            if (parent != nullptr) {
                if (parent->left == target) {
                    parent->left = nullptr;
                } else {
                    parent->right = nullptr;
                }
                _update_height(parent);
                _rebalance_after_delete(parent);
            }

            _real_delete(target);

            return nullptr;
        } else if (leftChild != nullptr && rightChild != nullptr) {
            // Case 3: Node has two children
            auto successor = BinaryTree_e::first_node(rightChild);
            _swap_node(target, successor);
            return _delete_by_target(successor);
        } else {
            // Case 2: Node has one child
            auto child = (leftChild != nullptr) ? leftChild : rightChild;
            child->parent = parent;
            if (parent != nullptr) {
                if (parent->left == target) {
                    parent->left = child;
                } else {
                    parent->right = child;
                }
                _update_height(parent);
                _rebalance_after_delete(parent);
            }

            _real_delete(target);

            return child;
        }
    }

    typename Node_::LinkType * _delete(typename Node_::LinkType *root, const T &obj) {
        auto nPtr = Node_::to_node(root);
        if (mCmp_d(obj, nPtr->data.val)) {
            root->left = _delete(root->left, obj);
        } else if (mCmp_d(nPtr->data.val, obj)) {
            root->right = _delete(root->right, obj);
        } else {

            // l and r isn't nullptr, need to find leaf-node(r-side)
            if (nullptr != root->left && nullptr != root->right) {
                // step1: find first node in right-sub-tree
                auto target = BinaryTree_e::first_node(root->right);
                // step2: swap root and target, then continue _delete
                _swap_node(root, target);
                root->right = _delete(root->right, obj); // retry
            } else {
                typename Node_::LinkType *subTree = nullptr;

                if (nullptr == root->left) {
                    subTree = root->right;
                } else if (nullptr == root->right) {
                    subTree = root->left;
                }

                if (subTree) {
                    subTree->parent = root->parent;
                }

                _real_delete(root);

                return subTree; // Note: only need return sub-tree directly
            }
        }

        _update_height(root);

        return _check_and_balance(root);
    }

protected: // helper
    int _height(typename Node_::LinkType *root) {
        if (root == nullptr)
            return 0;
        return Node_::to_node(root)->data.height;
    }

    void _update_height(typename Node_::LinkType *node) {
        int height = dstruct::max(_height(node->left), _height(node->right)) + 1;
        Node_::to_node(node)->data.height = height;
    }

    int _balance_factor(typename Node_::LinkType *root) {
        return root == nullptr ? 0 : _height(root->left) - _height(root->right);
    }

    void _real_delete(typename Node_::LinkType *linkPtr) {
        auto nodePtr = Node_::to_node(linkPtr);
        dstruct::destroy(nodePtr);
        AllocNode_::deallocate(nodePtr);
        BinaryTree_e::mSize_d--;
    }

    // only for support _delete / _delete_by_target operate
    void _swap_node(typename Node_::LinkType * &a, typename Node_::LinkType * &b) {

        // Ensure that a is the ancestor node of b
        if (a->parent == b) dstruct::swap(a, b);

        auto aParent = a->parent;
        auto aLeft = a->left;
        auto aRight = a->right;
        auto bParent = b->parent;
        auto bLeft = b->left;
        auto bRight = b->right;

        if (bParent == a) {

            b->parent = aParent;

            if (a->right == b) {
                // reset b 2 
                b->left = aLeft;
                b->right = a;
    
                if (aLeft) aLeft->parent = b;   // 6

            } else {
                // reset b 2 
                b->left = a;
                b->right = aRight;

                if (aRight) aRight->parent = b;   // 6
            }

            // reset a 1
            a->right = bRight;
            a->left = bLeft;
            a->parent = b;

            // aParent 3
            if (aParent && aParent->left == a) aParent->left = b;
            if (aParent && aParent->right == a) aParent->right = b;
            // bParent -
            if (bRight) bRight->parent = a; // 4
            if (bLeft) bLeft->parent = a;   // 5

        } else {
            /*
                |       |
                a       b
               / \     / \
            */

            // reset aParent
            if (aParent && aParent->left == a) aParent->left = b;
            if (aParent && aParent->right == a) aParent->right = b;
            b->parent = aParent;

            // reset bParent
            if (bParent && bParent->left == b) bParent->left = a;
            if (bParent && bParent->right == b) bParent->right = a;
            a->parent = bParent;

            // reset a's l/r
            if (aLeft) aLeft->parent = b;
            if (aRight) aRight->parent = b;
            b->left = aLeft; b->right = aRight;

            // reset b's l/r
            if (bLeft)  bLeft->parent = a;
            if (bRight) bRight->parent = a;
            a->left = bLeft; a->right = bRight;
        }

        // swap height
        dstruct::swap(Node_::to_node(a)->data.height, Node_::to_node(b)->data.height);
        dstruct::swap(a, b);

        // update root if root changed
        if (a->parent == nullptr) BinaryTree_e::_update_root(a);
        if (b->parent == nullptr) BinaryTree_e::_update_root(b);

    }

    typename Node_::LinkType * _left_rotate(typename Node_::LinkType *root) {
        root = tree::left_rotate(root);
        auto rootNode = Node_::to_node(root);
        auto leftNode = Node_::to_node(root->left);
        leftNode->data.height = dstruct::max(_height(leftNode->link.left), _height(leftNode->link.right)) + 1;
        rootNode->data.height = dstruct::max(leftNode->data.height, _height(rootNode->link.left)) + 1;
        return root;
    }

    typename Node_::LinkType * _right_rotate(typename Node_::LinkType *root) {
        root = tree::right_rotate(root);
        auto rootNode = Node_::to_node(root);
        auto rightNode = Node_::to_node(root->right);
        rightNode->data.height = dstruct::max(_height(rightNode->link.left), _height(rightNode->link.right)) + 1;
        rootNode->data.height = dstruct::max(_height(rootNode->link.left), rightNode->data.height) + 1;
        return root;
    }
};

}

#endif