// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#include <iostream>
#include <queue>

using std::queue;

#include <dstruct.hpp>
/*
using Node = dstruct::tree::EmbeddedBinaryTreeNode<dstruct::_AVLData<int>>;

void printTreeStructure(Node::LinkType *root) {
    if (!root) {
        return;
    }

    std::queue<Node::LinkType*> q;
    q.push(root);

    while (!q.empty()) {
        int levelSize = q.size();
        for (int i = 0; i < levelSize; ++i) {
            Node::LinkType* current = q.front();
            q.pop();

            if (current) {
                std::cout << Node::to_node(current)->data.val << " ";
                q.push(current->left);
                q.push(current->right);
            } else {
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }
}
*/

int main() {

    std::cout << "\nTesting: " << __FILE__;

    {
        dstruct::AVLTree<int, dstruct::less<int>, dstruct::Alloc> avlTree;

        for (int i = 10; i >= 0; i--) { // Test: R-Rotate
            avlTree.insert(i);
        }

        //printTreeStructure((Node::LinkType *)avlTree._get_root_ptr());

        for (int i = 0; i <= 10; i++) {
            avlTree.pop(i);
            //printTreeStructure((Node::LinkType *)avlTree._get_root_ptr());
        }

        DSTRUCT_ASSERT(avlTree.size() == 0);

        for (int i = 101; i <= 200; i++) { // Test: L-Rotate
            avlTree.insert(i);
        }
        for (int i = 1; i <= 100; i++) {
            avlTree.insert(500 + i);
            avlTree.insert(500 - i);
        }

        std::cout << avlTree.height() << std::endl;
        DSTRUCT_ASSERT(avlTree.height() <= 9 /* 2^9 = 512*/);
    }

    std::cout << "   pass" << std::endl;

    return 0;
}