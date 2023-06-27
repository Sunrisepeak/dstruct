#ifndef __BINARY_SEARCH_TREE_HPP__DSTRUCT
#define __BINARY_SEARCH_TREE_HPP__DSTRUCT

#include <core/common.hpp>

#include <core/ds/tree/Tree.hpp>

namespace dstruct {
namespace tree {

// TODO: https://en.cppreference.com/w/cpp/language/operator_comparison
template <typename T, typename CMP, typename Alloc>
class BinarySearchTree : public DStructTypeSpec<T, Alloc> {

protected:
    using _Node      = BinaryTreeNode<T>; 
    using _AllocNode = DSAlloc<_Node, Alloc>;

public: // big five

    BinarySearchTree(CMP cmp = CMP()) : _mSize { 0 }, _mCmp { cmp }, _mRootPtr { nullptr }  { };

    BinarySearchTree(const PrimitiveIterator<T> &begin, const PrimitiveIterator<T> &end, CMP cmp = CMP()) :
        BinarySearchTree(cmp) {
        for (auto it = begin; it != end; it++) {
            insert(*it);
        }
    }

    virtual ~BinarySearchTree() {
        if (_mRootPtr) {
            postorder_traversal(&(_mRootPtr->link), [](typename _Node::LinkType *linkPtr) {
                _Node *nPtr = _Node::to_node(linkPtr);
                destory(nPtr);
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

    // push/pop
    void insert(const T &obj) {
        auto newRootPtr = _insert(_Node::to_link(_mRootPtr), obj);
        _mRootPtr = _Node::to_node(newRootPtr);
        _mSize++;
    }

    void erase(const T &obj) {
        auto newRootPtr = _delete(_Node::to_link(_mRootPtr), obj);
        _mRootPtr = _Node::to_node(newRootPtr);
        // _mSize--; in _try_to_delete
    }

protected:
    size_t _mSize;
    CMP _mCmp; // TODO: https://en.cppreference.com/w/cpp/language/operator_comparison
    _Node *_mRootPtr;

    typename _Node::LinkType * _insert(typename _Node::LinkType *root, const T &obj) {

        if (root == nullptr) {
            auto nPtr = _AllocNode::allocate();
            construct(nPtr, _Node(obj));
            root = _Node::to_link(nPtr);
        } else {
            if (_mCmp(obj, _Node::to_node(root)->data)) {
                root->left = _insert(root->left, obj);
            } else {
                root->right = _insert(root->right, obj);
            }
        }

        return root;
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
            return linkPtr;
        }

        // real delete
        auto nPtr = _Node::to_node(linkPtr);
        destory(nPtr);
        _AllocNode::deallocate(nPtr);
        _mSize--;
        return subTree;
    }

};

}

}

#endif