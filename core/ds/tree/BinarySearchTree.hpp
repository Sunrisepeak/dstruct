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
            insert(*it);
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

    // push/pop
    void insert(const T &obj) {
        auto newRootPtr = _insert(_Node::to_link(_mRootPtr), obj);
        _mRootPtr = _Node::to_node(newRootPtr);
        _mSize++;
    }

    void erase(const T &obj) {
        if (_mSize == 0) return; // TODO: better method?
        auto newRootPtr = _delete(_Node::to_link(_mRootPtr), obj);
        _mRootPtr = _Node::to_node(newRootPtr);
        // _mSize--; in _try_to_delete
    }

public: // algo
    template <typename Callback>
    void traversal(TraversalType ttype, Callback &cb) const {
        auto cbWrapper = [&](typename _Node::LinkType *link) {
            typename  BinarySearchTree::ConstIteratorType::ValueType &data = _Node::to_node(link)->data;
            cb(data);
        };
        auto traversal_func = _get_traversal_func<Callback>(ttype);
        traversal_func(_Node::to_link(_mRootPtr), cbWrapper);
    }

public: // range-for and iterator

    typename BinarySearchTree::ConstIteratorType begin(TraversalType itType) const {
        return _create_iterator(_mRootPtr, itType);
    }

    typename BinarySearchTree::ConstIteratorType end(TraversalType itType) const {
        return _create_iterator(nullptr, itType);
    }

protected:
    size_t _mSize;
    CMP _mCmp;
    _Node *_mRootPtr;

    typename _Node::LinkType * _insert(typename _Node::LinkType *root, const T &obj) {

        if (root == nullptr) {
            auto nPtr = _AllocNode::allocate();
            dstruct::construct(nPtr, _Node(obj));
            root = _Node::to_link(nPtr);
        } else {
            bool flag = false;
            typename _Node::LinkType *newLinkPtr = nullptr;

            if (_mCmp(obj, _Node::to_node(root)->data)) {
                flag = root->left == nullptr;
                root->left = newLinkPtr = _insert(root->left, obj);
            } else {
                flag = root->left == nullptr;
                root->right = newLinkPtr = _insert(root->right, obj);
            }

            if (flag && newLinkPtr != nullptr)
                newLinkPtr->parent = root;
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

    typename BinarySearchTree::ConstIteratorType _create_iterator(TraversalType itType, _Node *nPtr) const {
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
        return typename BinarySearchTree::ConstIteratorType(nPtr, nextFunc);
    }

    template <typename _Callback>
    auto _get_traversal_func(TraversalType ttype) const -> decltype(preorder_traversal<_Callback>) {
        switch (ttype) {
            case TraversalType::PreOrder:
                return preorder_traversal<_Callback>;
            case TraversalType::InOrder:
                return inorder_traversal<_Callback>;
            case TraversalType::PostOrder:
                return postorder_traversal<_Callback>;
            default:
                return preorder_traversal<_Callback>;
        }
    }

};

}

}

#endif