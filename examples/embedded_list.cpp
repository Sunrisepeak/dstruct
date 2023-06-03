#include <iostream>

#include "dstruct.hpp"

// int: embeded singly linked list methods sets
using IntESList = dstruct::EListNode<int, dstruct::SLink>;

int main() {

    std::cout << "\n\nTesting: " << __FILE__ << std::endl;

    dstruct::_EmbeddedListNode<int/*, dstruct::DLink*/> _eDList;

    dstruct::EListNode<int, dstruct::SLink> *intESList = new IntESList(); // list or head-node

    IntESList::init(intESList);

    assert(IntESList::empty(intESList));

    // add element
    for (int i = 0; i < 10; i++) {
        auto node = new dstruct::EListNode<int, dstruct::SLink>();
        node->data = i;
        IntESList::add(intESList, node);
    }

    for (auto it = intESList->link.next; it != IntESList::to_link(intESList); it = it->next) {
        auto node = IntESList::to_node(it);
        std::cout << "--->" << node->data << std::endl;
    }

    // del element
    while (!IntESList::empty(intESList)) {
        IntESList::LinkType *linkPtr = intESList->link.next;
        auto node = IntESList::to_node(linkPtr);
        std::cout << node->data << std::endl;
        IntESList::del(intESList, node);
        delete node;
    }

    delete intESList;

    return 0;
}