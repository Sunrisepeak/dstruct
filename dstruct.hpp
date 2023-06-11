#ifndef __DSTRUCT_HPP__DSTRUCT
#define __DSTRUCT_HPP__DSTRUCT

#include <core/ds/static/Array.hpp>
#include <core/ds/static/EmbeddedList.hpp>
#include <core/ds/Vector.hpp>
#include <core/ds/adapter/Stack.hpp>
#include <core/ds/adapter/Queue.hpp>
#include <core/ds/Queue/DoubleEndedQueue.hpp>
#include <core/ds/LinkedList/DoublyLinkedlist.hpp>

#include <core/algorithm.hpp>

namespace dstruct {
    // Array

    // Vector

    // Deque
    template<typename T, size_t ArrSize = 32, typename Alloc = port::Alloc>
    using Deque = DoubleEndedQueue<T, ArrSize, Alloc>;

    // EmbeddedList
    using SLink = _SinglyLink;
    using DLink = _DoublyLink;
    template<typename T, typename Link = DLink>
    using EListNode = _EmbeddedListNode<T, DLink>;

    // DoublyLinkedlist
    template<typename T, typename Alloc = port::Alloc>
    using DList = DoublyLinkedlist<T, Alloc>;

    // Queue
    template<typename T>
    using Queue = adapter::Queue<T, DoubleEndedQueue<T, 32>>;

    // Stack
    template<typename T>
    using Stack = adapter::Stack<T, Vector<T>>;

};

#endif