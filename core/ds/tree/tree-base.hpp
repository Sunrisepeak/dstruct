#ifndef __TREE_HPP__DSTRUCT
#define __TREE_HPP__DSTRUCT

namespace dstruct {

namespace tree {


template<typename T, typename Link>
struct TreeNode {
    Link link;
    T data;

    using LinkType = Link;

    TreeNode() : link { 0 } { }
    TreeNode(const T &_data) : TreeNode() { data = _data; }

    static TreeNode * to_node(Link *link) {
        return reinterpret_cast<TreeNode *>(link);
    }

    static Link * to_link(TreeNode *node) {
        return &(node->link);
    }
};

} // namespace tree

} // namespace dstruct


#endif