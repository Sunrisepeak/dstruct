#ifndef __TREE_HPP__DSTRUCT
#define __TREE_HPP__DSTRUCT

namespace dstruct {

namespace tree {


template<typename T, typename Link>
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
        return static_cast<Link *>((void *)(data) - sizeof(Link));
    }
};

} // namespace tree

} // namespace dstruct


#endif