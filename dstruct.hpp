#ifndef __DSTRUCT_HPP__DSTRUCT
#define __DSTRUCT_HPP__DSTRUCT

#include <core/ds/Heap.hpp>

#include <core/ds/array/Vector.hpp>

// static
#include <core/ds/array/Array.hpp>
#include <core/ds/linked-list/EmbeddedList.hpp>

// stack
#include <core/ds/stack/Stack.hpp>
#include <core/ds/stack/XValueStack.hpp>

// queue
#include <core/ds/queue/Queue.hpp>
#include <core/ds/queue/DoubleEndedQueue.hpp>

// linked list
#include <core/ds/linked-list/DoublyLinkedlist.hpp>

// tree
#include <core/ds/tree/BinarySearchTree.hpp>

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
    template<typename T, typename Alloc = port::Alloc>
    using Queue = adapter::Queue<T, DoubleEndedQueue<T, 32, Alloc>>;

// Stack
    template<typename T, typename Alloc = port::Alloc>
    using Stack = adapter::Stack<T, Vector<T, Alloc>>;

    //template<typename T, typename Compare, typename StackType = adapter::Stack<T, Vector<T>>>
    //class XValueStack;
    template<typename T, typename Alloc = port::Alloc>
    using MinStack = stack::XValueStack<T, less<T>, adapter::Stack<T, Vector<T, Alloc>> >;
    template<typename T, typename Alloc = port::Alloc>
    using MaxStack = stack::XValueStack<T, greater<T>, adapter::Stack<T, Vector<T, Alloc>> >;


// Heap
    template<typename T, typename Alloc = port::Alloc>
    using MinHeap = Heap<T, less<T>, Alloc>;
    template<typename T, typename Alloc = port::Alloc>
    using MaxHeap = Heap<T, greater<T>, Alloc>;

// Tree
    //template <typename T, typename CMP, typename Alloc>
    //class BinarySearchTree;
    template <typename T, typename Alloc = port::Alloc>
    using BSTree = tree::BinarySearchTree<T, less<T>, Alloc>;
};

#endif