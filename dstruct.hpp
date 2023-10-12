// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef __DSTRUCT_HPP__DSTRUCT
#define __DSTRUCT_HPP__DSTRUCT

//#define ENABLE_SMA

#include <dstruct-static.hpp>

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
#include <core/ds/linked-list/SinglyLinkedList.hpp>
#include <core/ds/linked-list/DoublyLinkedList.hpp>

// String
#include <core/ds/string/BasicString.hpp>

// tree
#include <core/ds/tree/BinarySearchTree.hpp>

// set
#include <core/ds/set/DisjointSet.hpp>

#include <core/algorithm.hpp>

// other
#include <memory/StaticMemAllocator.hpp>

namespace dstruct {
    // Array

    // Vector
    
    // String
    using String = BasicString<char, dstruct::Alloc>;

// EmbeddedList
    template <typename T, typename Link = _DoublyLink>
    using EListNode = _EmbeddedListNode<T, Link>;

// SinglyLinkedList
    template <typename T, typename Alloc = dstruct::Alloc>
    using SLinkedList = SinglyLinkedList<T, Alloc>;

// DoublyLinkedList
    template <typename T, typename Alloc = dstruct::Alloc>
    using DLinkedList = DoublyLinkedList<T, Alloc>;

// Queue
    template <typename T, typename Alloc = dstruct::Alloc>
    using Queue = adapter::Queue<T, DoubleEndedQueue<T, 32, Alloc>>;
    template <typename T, size_t ArrSize = 32, typename Alloc = dstruct::Alloc>
    using Deque = DoubleEndedQueue<T, ArrSize, Alloc>;
    template <typename T, typename CMP = less<T>, typename Alloc = dstruct::Alloc>
    using PriorityQueue = Heap<T, CMP, Alloc>;

// Stack
    template <typename T, typename Alloc = dstruct::Alloc>
    using Stack = adapter::Stack<T, Vector<T, Alloc>>;

    //template <typename T, typename Compare, typename StackType = adapter::Stack<T, Vector<T>>>
    //class XValueStack;
    template <typename T, typename Alloc = dstruct::Alloc>
    using MinStack = stack::XValueStack<T, less<T>, adapter::Stack<T, Vector<T, Alloc>> >;
    template <typename T, typename Alloc = dstruct::Alloc>
    using MaxStack = stack::XValueStack<T, greater<T>, adapter::Stack<T, Vector<T, Alloc>> >;


// Heap
    template <typename T, typename Alloc = dstruct::Alloc>
    using MinHeap = Heap<T, less<T>, Alloc>;
    template <typename T, typename Alloc = dstruct::Alloc>
    using MaxHeap = Heap<T, greater<T>, Alloc>;

// Tree

    //template <typename T, typename CMP, typename Alloc>
    //class BinarySearchTree;
    template <typename T>
    using EBinaryTreeNode = tree::EmbeddedBinaryTreeNode<T>;
    template <typename T, typename Alloc = dstruct::Alloc>
    using BSTree = tree::BinarySearchTree<T, less<T>, Alloc>;

// Set
    using UFSet = DisjointSet<dstruct::Alloc>;
};

#endif