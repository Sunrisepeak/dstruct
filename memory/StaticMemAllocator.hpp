// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef __STATIC_MEM_ALLOCATOR_HPP__DSTRUCT
#define __STATIC_MEM_ALLOCATOR_HPP__DSTRUCT

#include <cstdio>
// only use no-dependency and static data-structures
#include <core/ds/array/Array.hpp>
#include <core/ds/linked-list/EmbeddedList.hpp>

namespace dstruct {

#define SMA_MEM_ALIGN 8
#define SMA_MEM_VERIFY(expr) __DSTRUCT_CRASH((expr) == nullptr)
#define SMA_POINTER_CMP(p1, p2) (reinterpret_cast<char *>(p1) == reinterpret_cast<char *>(p2))

/*
_mFreeMemList: when SMA_MEM_ALIGN equal 8
    +---------------------------+
    | 0 | 1 | ......... |n-1| X | - index
    +---------------------------+
    | p | p | ......... | p | X | - list-head 
    ----------------------------+
      |                  |     |
      V                  V     V 
    list                list  list-mem-pool            - list
    __Link             __Link  __MemBlock              - list-node
    8*(0+1)             8*n    __MemBlock->data        - mem-block size
*/


template <int MEMORY_SIZE, int MAX_BLOCK_SIZE = 128>
struct StaticMemAllocator {

private:
    using __Link = _SinglyLink;
    /*
        struct __MemBlock {
            __Link link;
            int data;
        }
    */
    using __MemBlock = dstruct::_EmbeddedListNode<int, __Link>;

private: // big five
    StaticMemAllocator() : _mFreeMemSize { MEMORY_SIZE }, _mMemoryPool { 0 }, _mFreeMemList(__Link{ nullptr }) {
        auto mbPtr = reinterpret_cast<__MemBlock *>(&_mMemoryPool);
        mbPtr->data = MEMORY_SIZE;
        mbPtr->link.next = nullptr;
        _mFreeMemList[-1].next = __MemBlock::to_link(mbPtr);
    }

    StaticMemAllocator(const StaticMemAllocator&) = delete;
    StaticMemAllocator & operator=(const StaticMemAllocator&) = delete;

public: // mem-alloc interface
    static void * allocate(int bytes) {
        return _Instance()._allocate(bytes);
    }

    static bool deallocate(void *addr, int bytes) {
        return _Instance()._deallocate(addr, bytes);
    }

public: // mem-manager interface

    constexpr static int MEM_ALIGN_ROUND_UP(int bytes) {
        return (((bytes) + SMA_MEM_ALIGN - 1) & ~(SMA_MEM_ALIGN - 1));
    }

    static int free_mem_size() {
        return _Instance()._mFreeMemSize;
    }

    static int max_free_mblock_size() {
        // check - mem-pool list
        auto  *linkPtr = _Instance()._mFreeMemList.back().next;
        int maxSize = 0;
        while (linkPtr != nullptr) {
            auto mbPtr = __MemBlock::to_node(linkPtr);
            if (maxSize < mbPtr->data) maxSize = mbPtr->data;
            linkPtr = linkPtr->next;
        }

        if (maxSize > 0) return maxSize;

        // check - mem-block list
        int listIndex = _Instance()._mFreeMemList.size() - 1 - 1;
        while (listIndex >= 0 && _Instance()._mFreeMemList[listIndex].next == nullptr) {
            listIndex--;
        }

        maxSize = (listIndex + 1) * SMA_MEM_ALIGN;

        return maxSize;
    }

    static void memory_merge() {
        _Instance()._memory_merge();
    }

    static void dump(const char *str) {
        printf("sma dump(total %d, used %d, free %d) - %s: \n",
            MEMORY_SIZE, MEMORY_SIZE - _Instance()._mFreeMemSize, _Instance()._mFreeMemSize, str);
        int verifyFreeMemSize = 0;
        for (int i = 0; i < _Instance()._mFreeMemList.size(); i++) {
            __Link *mbPtr = _Instance()._mFreeMemList[i].next;
            int lIndex = 0;
            while (mbPtr != nullptr) {
                int size = 0;
                if (i == _Instance()._mFreeMemList.size() - 1) {
                    size = __MemBlock::to_node(mbPtr)->data;
                } else {
                    size = _TO_SIZE(i);
                }
                printf("\tt-index: %d, l-index %d, addr %p, size %d\n", i, lIndex++, mbPtr, size);
                verifyFreeMemSize += size;
                mbPtr = mbPtr->next;
            }
        }

        printf("sma dump free-mem verify: %d == %d\n", _Instance()._mFreeMemSize, verifyFreeMemSize);

        __DSTRUCT_CRASH(_Instance()._mFreeMemSize != verifyFreeMemSize);
    }

protected:
    int _mFreeMemSize;
    char _mMemoryPool[MEMORY_SIZE];
    dstruct::Array<__Link, MAX_BLOCK_SIZE / SMA_MEM_ALIGN + 1/*MemPoolManager*/> _mFreeMemList;

    static StaticMemAllocator & _Instance() {
        static StaticMemAllocator sma; // create & manage static memory area
        return sma;
    }

    constexpr static int _TO_INDEX(int bytes) {
        return MEM_ALIGN_ROUND_UP(bytes) / SMA_MEM_ALIGN - 1;
    }

    constexpr static int _TO_SIZE(int index) {
        return (index + 1) * SMA_MEM_ALIGN;
    }

    void * _allocate(int bytes) {

        int freeMemListIndex = bytes > MAX_BLOCK_SIZE ? _mFreeMemList.size() - 1 : _TO_INDEX(bytes);

        while ( // search free memory block
            freeMemListIndex < _mFreeMemList.size() &&
            _mFreeMemList[freeMemListIndex].next == nullptr
        ) freeMemListIndex++;

        if (freeMemListIndex == _mFreeMemList.size()) {
            return nullptr;
        }

        // preLinkPtr
        __Link *preLinkPtr = &(_mFreeMemList[freeMemListIndex]);
        if (freeMemListIndex == _mFreeMemList.size() - 1) { // search mem-pool
            while (preLinkPtr->next != nullptr) {
                auto mbPtr = __MemBlock::to_node(preLinkPtr->next);
                if (mbPtr->data >= MEM_ALIGN_ROUND_UP(bytes))
                    break;
                preLinkPtr = preLinkPtr->next;
            }
        }

        __Link *memPtr = preLinkPtr->next;
        if (memPtr == nullptr) return nullptr;
        // delete memory block
        preLinkPtr->next = memPtr->next;

        int allocatedSize = MEM_ALIGN_ROUND_UP(bytes);
        void *memFragmentAddr = reinterpret_cast<char *>(memPtr) + allocatedSize;

        // check and insert memory fragment to list
        if (freeMemListIndex == _mFreeMemList.size() - 1) { // mem-pool
            auto mbPtr = __MemBlock::to_node(memPtr);
            _insert_mem_block_to_list(memFragmentAddr, mbPtr->data - allocatedSize);
        } else {
            int size = _TO_SIZE(freeMemListIndex) - allocatedSize;
            _insert_mem_block_to_list(memFragmentAddr, size);
        }

        _mFreeMemSize -= allocatedSize;

        return memPtr;
    }

    bool _deallocate(void *addr, int bytes) {
        if (bytes <= 0 || addr == nullptr) return false;
        _insert_mem_block_to_list(addr, MEM_ALIGN_ROUND_UP(bytes));
        _mFreeMemSize += MEM_ALIGN_ROUND_UP(bytes);
        return true;
    }

    void _memory_merge() {

        if (_mFreeMemList.back().next != nullptr) return;

        for (int i = 0; i < _mFreeMemList.size() - 1; i++) {
            auto firstMemLinkPtr = &(_mFreeMemList[i]);
            __Link *firstMemBlockPtr = firstMemLinkPtr->next;
            __Link *secondMemBlockPtr = nullptr;
            while (firstMemBlockPtr) {
                secondMemBlockPtr = firstMemBlockPtr->next;
                if (SMA_POINTER_CMP(reinterpret_cast<char *>(firstMemBlockPtr) + (i + 1) * SMA_MEM_ALIGN, secondMemBlockPtr)) {
                    // merge firstMemBlockPtr and secondMemBlockPtr
                    firstMemLinkPtr->next = secondMemBlockPtr->next; // delete first/secondMemBlockPtr from list[i]
                    int listIndex = i * 2 + 1;
                    //printf("memory_merge: [%p, %p], from list %d\n", firstMemBlockPtr, secondMemBlockPtr, i);
                    _insert_mem_block_to_list(firstMemBlockPtr, _TO_SIZE(listIndex));
                } else {
                    bool secondMemBlockFlag = false;
                    for (int j = i + 1; j < _mFreeMemList.size() - 1 && !secondMemBlockFlag; j++) {
                        auto secondMemLinkPtr = &(_mFreeMemList[j]);
                        secondMemBlockPtr = secondMemLinkPtr->next;
                        while (secondMemBlockPtr != nullptr) {
                            /*                         +-----+ 
                              xUp                -->   |     |
                              firstMemBlockPtr   -->   |     |
                              xDown              -->   |     |
                                                       +-----+
                            */
                            void *addrUP = reinterpret_cast<char *>(firstMemBlockPtr) - (j + 1) * SMA_MEM_ALIGN;
                            void *addrDown = reinterpret_cast<char *>(firstMemBlockPtr) + (i + 1) * SMA_MEM_ALIGN;

                            if (SMA_POINTER_CMP(addrUP, secondMemBlockPtr) || SMA_POINTER_CMP(addrDown, secondMemBlockPtr)) { // check merge-able
                                firstMemLinkPtr->next = firstMemBlockPtr->next; // delete firstMemBlock from list[i]
                                secondMemLinkPtr->next = secondMemBlockPtr->next; // delete secondMemBlock from list[j]
                                _insert_mem_block_to_list(
                                    firstMemBlockPtr < secondMemBlockPtr ? firstMemBlockPtr : secondMemBlockPtr,
                                    _TO_SIZE(i + j + 1)
                                );
                                secondMemBlockFlag = true;
                                break;
                            }

                            if (firstMemBlockPtr > secondMemBlockPtr) break;

                            secondMemLinkPtr = secondMemBlockPtr;
                            secondMemBlockPtr = secondMemLinkPtr->next;
                        }
                    }

                    if (secondMemBlockFlag == false) // update firstMemLinkPtr
                        firstMemLinkPtr = firstMemBlockPtr;
                }

                firstMemBlockPtr = firstMemLinkPtr->next;
            }
        }

        _mem_pool_memory_merge();
    }

    void _insert_mem_block_to_list(void *addr, int size) {

        if (size <= 0) return;

        int ListIndex = _TO_INDEX(size);

        if (ListIndex >= _mFreeMemList.size() - 1) {
            ListIndex = _mFreeMemList.size() - 1;
        }

        __Link *freeMemLinkPtr = &(_mFreeMemList[ListIndex]);
        while (freeMemLinkPtr->next != nullptr && addr > freeMemLinkPtr->next) {
            freeMemLinkPtr = freeMemLinkPtr->next;
        }

        __Link::add(freeMemLinkPtr, static_cast<__Link *>(addr));

        if (ListIndex == _mFreeMemList.size() - 1) {
            // insert to MemoryPool, so need to set mem-block size
            __MemBlock *mbPtr = __MemBlock::to_node(freeMemLinkPtr->next);
            mbPtr->data = size;
        }

    }

    void _mem_pool_memory_merge() {
        int id = 0;
        __MemBlock *mbPtr = __MemBlock::to_node(_mFreeMemList.back().next);
        while (mbPtr != nullptr) {
            auto next = __MemBlock::to_node(mbPtr->link.next);
            if (SMA_POINTER_CMP(reinterpret_cast<char *>(mbPtr) + mbPtr->data, next)) {
                // merge next to mbPtr
                mbPtr->data += next->data;
                mbPtr->link.next = next->link.next;
            } else {
                id++;
                mbPtr = next;
            }
        }
    }
};

}

#endif