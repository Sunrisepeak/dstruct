#ifndef __DSTRUCT_HPP__DSTRUCT
#define __DSTRUCT_HPP__DSTRUCT

#include <ds/static/Array.hpp>
#include <ds/static/EmbeddedList.hpp>
#include <ds/Vector.hpp>
#include <ds/LinkedList/DoublyLinkedlist.hpp>


namespace dstruct {
    // Array

    // Vector

    // EmbeddedList
    using SLink = _SinglyLink;
    using DLink = _DoublyLink;
    template<typename T, typename Link = DLink>
    using EListNode = _EmbeddedListNode<T, DLink>;

    // DoublyLinkedlist
    template<typename T, typename Alloc = port::Alloc>
    using DList = DoublyLinkedlist<T, Alloc>;

};

#endif