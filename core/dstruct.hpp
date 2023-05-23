#ifndef __DSTRUCT_HPP__DSTRUCT
#define __DSTRUCT_HPP__DSTRUCT

#include <static/Array.hpp>
#include <static/EmbeddedList.hpp>
#include <Vector.hpp>
#include <LinkedList/DoublyLinkedlist.hpp>


namespace dstruct {
    // Array

    // Vector

    // EmbeddedList
    using SLink = _SinglyLink;
    using DLink = _DoublyLink;
    template<typename T, typename Link = DLink>
    using EList = _EmbeddedList<T, DLink>;

    // DoublyLinkedlist
    template<typename T, typename Alloc = port::Alloc>
    using DList = DoublyLinkedlist<T, Alloc>;

};

#endif