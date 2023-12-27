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


int main() {

    std::cout << "\nTesting: " << __FILE__;

    {
        dstruct::AVLTree<int, dstruct::less<int>, dstruct::Alloc> avlTree;
        dstruct::AVLTree<const double, dstruct::less<const double>, dstruct::Alloc> avlTree2;
        avlTree2.push(2.3);

        for (int i = 10; i >= 0; i--) { // Test: R-Rotate
            avlTree.push(i);
        }

        //printTreeStructure((Node::LinkType *)avlTree._get_root_ptr());

        for (int i = 0; i <= 10; i++) {// Test: delete
            avlTree.pop(i);
            //printTreeStructure((Node::LinkType *)avlTree._get_root_ptr());
        }

        DSTRUCT_ASSERT(avlTree.empty());

        for (int i = 101; i <= 200; i++) { // Test: L-Rotate
            avlTree.push(i);
        }

        //printTreeStructure((Node::LinkType *)avlTree._get_root_ptr());

        auto it = avlTree.find(150);
        DSTRUCT_ASSERT(it != avlTree.end());

        int val = *it;
        while (it != avlTree.end()) {
            //std::cout << *it << " == " << val << std::endl;
            DSTRUCT_ASSERT(*it == val);
            // *it = 3; // test const
            it++;
            val++;
        }

        for (int i = 1; i <= 100; i++) {
            avlTree.push(500 + i);
            avlTree.push(500 - i);
        }

        //std::cout << avlTree.height() << " - " << avlTree.size() << std::endl;

        //printTreeStructure((Node::LinkType *)avlTree._get_root_ptr());

        DSTRUCT_ASSERT(avlTree.height() <= 9 /* 2^9 = 512*/);

        int sum { 0 };
        avlTree.traversal(
            [&](dstruct::_AVLData<int> data) {
                sum++;
            },
            decltype(avlTree)::TraversalType::PreOrder
        );

        DSTRUCT_ASSERT(sum == 100 + 2 * 100);

    }

    // test _AVLData
    {
        struct A {
            int a;
            short b;
            char c;
        };

        A data {1, 2, 'a'};
        auto cmp = [](const A &a, const A &b) { return a.a < b.a; };
        dstruct::AVLTree<A, decltype(cmp), dstruct::Alloc> avlTree(cmp);

        avlTree.push(data);
        auto it = avlTree.find(data);
        DSTRUCT_ASSERT(it != avlTree.end() && it->c == data.c);

        //it->c = 'b'; // only-read
    }
    std::cout << "   pass" << std::endl;

    return 0;
}