#include <iostream>

#include "dstruct.hpp"

// int: embeded singly linked list methods sets
using IntESList = dstruct::EList<int, dstruct::SLink>;

int main() {

    std::cout << "\n\nTesting: " << __FILE__ << std::endl;

    dstruct::_EmbeddedList<int/*, dstruct::SLink*/> _eDList;

    dstruct::EList<int, dstruct::SLink> *intESList = new IntESList(); // list or head-node

    IntESList::init(intESList);

    assert(IntESList::empty(IntESList::to_link(intESList)));

    // add element
    for (int i = 0; i < 10; i++) {
        auto node = new dstruct::EList<int>();
        node->data = i;
        IntESList::add(&(intESList->link), IntESList::to_link(node));
    }

    // del element
    while (!IntESList::empty(IntESList::to_link(intESList))) {
        IntESList::LinkType *linkPtr = intESList->link.next;
        auto node = IntESList::to_node(linkPtr);
        std::cout << node->data << std::endl;
        IntESList::del(IntESList::to_link(intESList), linkPtr);
        delete node;
    }

    delete intESList;

    return 0;
}