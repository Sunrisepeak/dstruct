#ifndef __DSTRUCT_HPP__DSTRUCT
#define __DSTRUCT_HPP__DSTRUCT

#include <ds/static/Array.hpp>
#include <ds/static/EmbeddedList.hpp>
#include <ds/Vector.hpp>
#include <ds/adapter/Stack.hpp>
#include <ds/adapter/Queue.hpp>
#include <ds/Queue/DoubleEndedQueue.hpp>
#include <ds/LinkedList/DoublyLinkedlist.hpp>


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