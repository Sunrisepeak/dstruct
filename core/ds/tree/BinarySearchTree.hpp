#ifndef __BINARY_SEARCH_TREE_HPP__DSTRUCT
#define __BINARY_SEARCH_TREE_HPP__DSTRUCT

#include <core/common.hpp>
#include <core/ds/tree/EmbeddedBinaryTree.hpp>
#include <core/ds/tree/BinaryTree.hpp>

namespace dstruct {
namespace tree {

// TODO: https://en.cppreference.com/w/cpp/language/operator_comparison
template <typename T, typename CMP, typename Alloc>
class BinarySearchTree : public DStructTypeSpec<T, Alloc, _BinaryTreeIterator> {

public: // type
    enum TraversalType : uint8_t {
        PreOrder,
        InOrder,
        PostOrder,
    };

protected:
    using _Node      = EmbeddedBinaryTreeNode<T>; 
    using _AllocNode = DSAlloc<_Node, Alloc>;

public: // big five

    BinarySearchTree(CMP cmp = CMP()) : _mSize { 0 }, _mCmp { cmp }, _mRootPtr { nullptr }  { };

    BinarySearchTree(const PrimitiveIterator<T> &begin, const PrimitiveIterator<T> &end, CMP cmp = CMP()) :
        BinarySearchTree(cmp) {
        for (auto it = begin; it != end; it++) {
            push(*it);
        }
    }

    virtual ~BinarySearchTree() {
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

    typename BinarySearchTree::ConstIteratorType
    find(const T &obj) const {
        auto target = _find_or_insert(_Node::to_link(_mRootPtr), obj, _mCmp, false);
        return _create_iterator(target, TraversalType::InOrder);
    }

    // push/pop
    void push(const T &obj) {
        auto tree = _find_or_insert(_Node::to_link(_mRootPtr), obj, _mCmp, true);
        if (_mRootPtr == nullptr)
            _update_root(tree);
        _mSize++;
    }

    void pop(const T &obj) {
        if (_mSize == 0) return; // TODO: better method?
        auto root = _delete(_Node::to_link(_mRootPtr), obj);
        if (_mRootPtr != _Node::to_node(root)) {
            _update_root(root);
        }
        // _mSize--; in _try_to_delete
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
                _update_root(tree);
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

public: // algo
    template <typename Callback>
    void traversal(Callback cb, TraversalType ttype = TraversalType::InOrder) const {

        auto cbWrapper = [&](typename _Node::LinkType *link) {
            typename  BinarySearchTree::ConstIteratorType::ValueType &data = _Node::to_node(link)->data;
            cb(data);
        };

        switch (ttype) {
            case TraversalType::PreOrder:
                return  tree::preorder_traversal(_begin(_Node::to_link(_mRootPtr), ttype), cbWrapper);
            case TraversalType::InOrder:
                return  tree::inorder_traversal(_begin(_Node::to_link(_mRootPtr), ttype), cbWrapper);
            case TraversalType::PostOrder:
                return  tree::postorder_traversal(_begin(_Node::to_link(_mRootPtr), ttype), cbWrapper);
            default: {
                DSTRUCT_ASSERT(false);
            }
        }

        return  tree::preorder_traversal(_begin(_Node::to_link(_mRootPtr), ttype), cbWrapper);
    }

public: // range-for and iterator

    typename BinarySearchTree::ConstIteratorType begin(TraversalType ttype = TraversalType::InOrder) const {
        return typename BinarySearchTree::ConstIteratorType(
            _create_iterator(_begin(_Node::to_link(_mRootPtr), ttype), ttype),
            true
        );
    }

    typename BinarySearchTree::ConstIteratorType end(TraversalType ttype = TraversalType::InOrder) const {
        return typename BinarySearchTree::ConstIteratorType(
            _create_iterator(nullptr, ttype),
            true
        );
    }

public: // static pub api

    static typename _Node::LinkType * _begin(typename _Node::LinkType *root, TraversalType ttype = TraversalType::InOrder) {
        auto first = root;
        if (ttype != TraversalType::PreOrder) {
            while (first->left != nullptr) {
                DSTRUCT_ASSERT(first->left->parent == first);
                first = first->left;
            }
        }
        return first;
    }

    static typename _Node::LinkType * _find_or_insert(
        typename _Node::LinkType *root,
        const T &obj,
        CMP cmp = CMP(),
        bool insertFlag = false
    ) {
        typename _Node::LinkType * target = root;
        typename _Node::LinkType * parent = nullptr;

        while (target != nullptr) { // find
            parent = target;
            if (cmp(obj, _Node::to_node(target)->data)) {
                target = target->left;
            } else if (cmp(_Node::to_node(target)->data, obj)) {
                target = target->right;
            } else {
                // TODO: pls check your cmp, a < b, b < a, but a != b
                break;
            }
        }

        if (!insertFlag) return target;

        // create node
        auto nodePtr = _AllocNode::allocate();
        auto newNodeLink = _Node::to_link(nodePtr);
        dstruct::construct(nodePtr, _Node(obj));

        if (parent != nullptr) { // nodePtr isn't root
            newNodeLink->parent = parent;
            decltype(parent) subTree = nullptr; // for readability
            if (cmp(obj, _Node::to_node(parent)->data)) {
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
        dstruct::destory(nPtr);
        _AllocNode::deallocate(nPtr);
        _mSize--;
        return subTree;
    }

    typename BinarySearchTree::IteratorType _create_iterator(typename _Node::LinkType *link, TraversalType itType) const {
        typename BinarySearchTree::IteratorType::NextFunc nextFunc = nullptr;
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
        return typename BinarySearchTree::IteratorType(link, nextFunc);
    }

};

}

}

#endif