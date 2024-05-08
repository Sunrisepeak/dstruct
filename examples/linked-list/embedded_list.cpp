// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#include <iostream>

#include <dstruct.hpp>

// int: embeded singly linked list methods sets
using IntESList = dstruct::EListNode<int, dstruct::SinglyLink_>;

int main() {

    std::cout << "\nTesting: " << __FILE__;

    dstruct::EmbeddedListNode_<int/*, dstruct::DLink*/> list;

    dstruct::EListNode<int, dstruct::SinglyLink_> *intESList = new IntESList(); // list or head-node

    IntESList::init(intESList);

    DSTRUCT_ASSERT(IntESList::empty(intESList));

    // add element
    for (int i = 0; i < 10; i++) {
        auto node = new dstruct::EListNode<int, dstruct::SinglyLink_>();
        node->data = i;
        IntESList::add(intESList, node);
    }

    int data { 9 };
    for (auto it = intESList->link.next; it != IntESList::to_link(intESList); it = it->next) {
        auto node = IntESList::to_node(it);
        //std::cout << "--->" << node->data << std::endl;
        DSTRUCT_ASSERT(node->data == data);
        data--;
    }

    // del element
    while (!IntESList::empty(intESList)) {
        IntESList::LinkType *linkPtr = intESList->link.next;
        auto node = IntESList::to_node(linkPtr);
        //std::cout << node->data << std::endl;
        IntESList::del(intESList, node);
        delete node;
    }

    delete intESList;

    std::cout << "   pass" << std::endl;

    return 0;
}