#ifndef __TREE_HPP__DSTRUCT
#define __TREE_HPP__DSTRUCT

#include <core/common.hpp>

#include <core/ds/tree/BinaryTree.hpp>

namespace dstruct {
namespace tree {

template<typename T, typename Link>
struct TreeNode {
    Link link;
    T data;

    static TreeNode * to_node(Link *link) {
        return reinterpret_cast<TreeNode *>(link);
    }

    static Link * to_link(TreeNode *node) {
        return &(node->link);
    }
};

}

}

#endif