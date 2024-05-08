// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef DSTRUCT_STATIC_HPP__DSTRUCT
#define DSTRUCT_STATIC_HPP__DSTRUCT

#include <core/ds/Heap.hpp>

// Array
#include <core/ds/array/Array.hpp>
#include <core/ds/array/Vector.hpp>

// stack
#include <core/ds/stack/Stack.hpp>
#include <core/ds/stack/XValueStack.hpp>

// queue
#include <core/ds/queue/Queue.hpp>
#include <core/ds/queue/DoubleEndedQueue.hpp>

// linked list
#include <core/ds/linked-list/EmbeddedList.hpp>
#include <core/ds/linked-list/SinglyLinkedList.hpp>
#include <core/ds/linked-list/DoublyLinkedList.hpp>

// String
#include <core/ds/string/BasicString.hpp>

// tree
#include <core/ds/tree/BinarySearchTree.hpp>

// set
#include <core/ds/set/DisjointSet.hpp>

// other
#include <core/algorithm.hpp>
#include <memory/StaticMemAllocator.hpp>

namespace dstruct {

namespace smemory {
// SMA
    #define SMA_MEM_SIZE 1024 * 10 // 10K
    using SMA = dstruct::StaticMemAllocator<SMA_MEM_SIZE>;

// Array
    template <typename T, size_t N>
    using Array = dstruct::Array<T, N>;

// Vector
    template <typename T>
    using Vector = dstruct::Vector<T, SMA>;

// String
    using String = BasicString<char, SMA>;

// EmbeddedList
    template <typename T, typename Link = _DoublyLink>
    using EListNode = _EmbeddedListNode<T, Link>;

// SinglyLinkedList
    template <typename T>
    using SLinkedList = SinglyLinkedList<T, SMA>;

// DoublyLinkedList
    template <typename T>
    using DLinkedList = DoublyLinkedList<T, SMA>;

// Queue
    template <typename T>
    using Queue = adapter::Queue<T, dstruct::DoubleEndedQueue<T, 32, SMA>>;
    template <typename T, size_t ArrSize = 32>
    using Deque = DoubleEndedQueue<T, ArrSize, SMA>;
    template <typename T, typename CMP = less<T>>
    using PriorityQueue = Heap<T, CMP, SMA>;

// Stack
    template <typename T>
    using Stack = adapter::Stack<T, smemory::Vector<T>>;
    template <typename T>
    using MinStack = stack::XValueStack<T, less<T>, smemory::Stack<T>>;
    template <typename T>
    using MaxStack = stack::XValueStack<T, greater<T>, smemory::Stack<T>>;

// Heap
    template <typename T>
    using MinHeap = Heap<T, less<T>, SMA>;
    template <typename T>
    using MaxHeap = Heap<T, greater<T>, SMA>;

// Tree
    template <typename T>
    using EBinaryTreeNode = tree::EmbeddedBinaryTreeNode<T>;
    template <typename T>
    using BSTree = tree::BinarySearchTree<T, less<T>, SMA>;

// Set
    using UFSet = DisjointSet<SMA>;

}

}

#endif