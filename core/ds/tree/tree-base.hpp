// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef __TREE_BASE_HPP__DSTRUCT
#define __TREE_BASE_HPP__DSTRUCT

namespace dstruct {

namespace tree {

template <typename T, typename Link>
struct TreeNode {
    Link link;
    T data;

    using LinkType = Link;
    // big five
    TreeNode() : link {} {  }
    TreeNode(const T &_data) : TreeNode() { data = _data; }
    TreeNode(const TreeNode &tNode) { data = tNode.data; link = tNode.link;  }

    static TreeNode * to_node(Link *link) {
        return reinterpret_cast<TreeNode *>(link);
    }

    static Link * to_link(TreeNode *node) {
        return &(node->link);
    }

    static Link * data_to_link(const T *data) {
        // TODO: (void *) verify/test
        return reinterpret_cast<Link *>((int8_t *)(data) - sizeof(Link));
    }
};

} // namespace tree

} // namespace dstruct


#endif