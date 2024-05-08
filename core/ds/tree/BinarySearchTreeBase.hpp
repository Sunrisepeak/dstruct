// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef BINARY_SEARCH_TREE_BASE_HPP_DSTRUCT
#define BINARY_SEARCH_TREE_BASE_HPP_DSTRUCT

#include <core/ds/tree/EmbeddedBinaryTree.hpp>

namespace dstruct {

template <typename T, typename CMP>
struct BinarySearchTreeBase {
    using Node = tree::EmbeddedBinaryTreeNode<T>;

    static typename Node::LinkType *
    _find(typename Node::LinkType *root, const T &obj, CMP cmp = CMP()) {
        typename Node::LinkType * target = root;

        while (target != nullptr) { // find
            if (cmp(obj, Node::to_node(target)->data)) {
                target = target->left;
            } else if (cmp(Node::to_node(target)->data, obj)) {
                target = target->right;
            } else {
                break;
            }
            //printf("%d: %d\n", obj.val, Node::to_node(target)->data.val);
        }

        return target;
    }
};

}

#endif