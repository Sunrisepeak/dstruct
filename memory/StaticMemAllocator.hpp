// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef STATIC_MEM_ALLOCATOR_HPP__DSTRUCT
#define STATIC_MEM_ALLOCATOR_HPP__DSTRUCT

#include <cstdio>
// only use no-dependency and static data-structures
#include <core/ds/array/Array.hpp>
#include <core/ds/linked-list/EmbeddedList.hpp>

namespace dstruct {

#define SMA_MEM_ALIGN 8
#define SMA_MEM_VERIFY(expr) DSTRUCT_CRASH((expr) == nullptr)
#define SMA_ADDRESS_EQUAL(p1, p2) (reinterpret_cast<char *>(p1) == reinterpret_cast<char *>(p2))

#ifndef SMA_LOGD
#define SMA_LOGD(...) // no-impl
#endif


/*
_mFreeMemList: when SMA_MEM_ALIGN equal 8
    +---------------------------+
    | 0 | 1 | ......... |n-1| X | - index
    +---------------------------+
    | l | l | ......... | l | p | - list-head 
    ----------------------------+
      |                  |     |
      V                  V     V 
    list                list  list-mem-pool            - list
    __Link             __Link  __MemBlock              - list-node
    8*(0+1)             8*n    __MemBlock->data        - mem-block size
*/

// Note: request MEMORY_SIZE % SMA_MEM_ALIGN == 0
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
        // check - mem-pool
        auto  *linkPtr = _Instance()._mFreeMemList.back().next;
        int maxSize = 0;
        while (linkPtr != nullptr) {
            auto mbPtr = __MemBlock::to_node(linkPtr);
            if (maxSize < mbPtr->data) maxSize = mbPtr->data;
            linkPtr = linkPtr->next;
        }

        if (maxSize > 0) return maxSize;

        // check - quick-mem
        int listIndex = _Instance()._mFreeMemList.size() - 1 - 1;
        while (listIndex >= 0 && _Instance()._mFreeMemList[listIndex].next == nullptr) {
            listIndex--;
        }

        maxSize = _INDEX_TO_SIZE(listIndex);

        return maxSize;
    }

    static void memory_merge() {
        _Instance()._memory_merge();
    }

    static void dump() {
        SMA_LOGD("sma dump(total %d, used %d, free %d):",
            MEMORY_SIZE, MEMORY_SIZE - _Instance()._mFreeMemSize, _Instance()._mFreeMemSize);
        int verifyFreeMemSize = 0;
        for (int i = 0; i < _Instance()._mFreeMemList.size(); i++) {
            __Link *mbPtr = _Instance()._mFreeMemList[i].next;
            int lIndex = 0;
            while (mbPtr != nullptr) {
                int size = 0;
                if (i == _Instance()._mFreeMemList.size() - 1) {
                    size = __MemBlock::to_node(mbPtr)->data;
                } else {
                    size = _INDEX_TO_SIZE(i);
                }
                SMA_LOGD("\tt-index: %d, l-index %d, addr %p, size %d", i, lIndex++, mbPtr, size);
                verifyFreeMemSize += size;
                mbPtr = mbPtr->next;
            }
        }

        SMA_LOGD("\tfree-mem verify: %d == %d", _Instance()._mFreeMemSize, verifyFreeMemSize);

        DSTRUCT_CRASH(_Instance()._mFreeMemSize != verifyFreeMemSize);
    }

protected:
    int _mFreeMemSize;
    char _mMemoryPool[MEMORY_SIZE];
    dstruct::Array<__Link, MAX_BLOCK_SIZE / SMA_MEM_ALIGN + 1/*MemPoolManager*/> _mFreeMemList;

    static StaticMemAllocator & _Instance() {
        static StaticMemAllocator sma; // create & manage static memory area
        return sma;
    }

    constexpr static int _SIZE_TO_INDEX(int bytes) {
        return MEM_ALIGN_ROUND_UP(bytes) / SMA_MEM_ALIGN - 1;
    }

    constexpr static int _INDEX_TO_SIZE(int index) {
        return (index + 1) * SMA_MEM_ALIGN;
    }

    void * _allocate(int bytes) {

        __MemBlock targetMemBlockIndex;

        if (bytes > MAX_BLOCK_SIZE) {
            targetMemBlockIndex = _mem_pool_allocate(bytes);
        } else {
            targetMemBlockIndex = _quick_mem_allocate(bytes);
            if (targetMemBlockIndex.link.next == nullptr)
                targetMemBlockIndex = _mem_pool_allocate(bytes);
        }

        __Link *memPtr = targetMemBlockIndex.link.next;

        if (memPtr == nullptr) return nullptr;

        int allocatedSize = MEM_ALIGN_ROUND_UP(bytes);
        void *memFragmentAddr = reinterpret_cast<char *>(memPtr) + allocatedSize;
        int memFragmentSize = targetMemBlockIndex.data - allocatedSize;

        // insert memory fragment to list
        _insert_mem_block_to_list(memFragmentAddr, memFragmentSize);

        _mFreeMemSize -= allocatedSize;

        return memPtr;
    }

    bool _deallocate(void *addr, int bytes) {
        if (bytes <= 0 || addr == nullptr) return false;
        _insert_mem_block_to_list(addr, MEM_ALIGN_ROUND_UP(bytes));
        _mFreeMemSize += MEM_ALIGN_ROUND_UP(bytes);
        return true;
    }

protected: // quick allocator
    // request: bytes <= MAX_BLOCK_SIZE
    __MemBlock _quick_mem_allocate(int bytes) {

        __MemBlock targetMemIndex;

        bytes = MEM_ALIGN_ROUND_UP(bytes);

        int freeMemListIndex = _SIZE_TO_INDEX(bytes);
        while ( // search free memory block
            freeMemListIndex < _mFreeMemList.size() - 1 &&
            _mFreeMemList[freeMemListIndex].next == nullptr
        ) freeMemListIndex++;

        if (freeMemListIndex == _mFreeMemList.size() - 1) {
            targetMemIndex.link.next = nullptr;
            targetMemIndex.data = 0;
        } else {
            // fill mem block info(addr and size) to targetMemIndex
            auto targetMemBlockLinkPtr = _mFreeMemList[freeMemListIndex].next;
            targetMemIndex.link.next = targetMemBlockLinkPtr;
            targetMemIndex.data = _INDEX_TO_SIZE(freeMemListIndex);
            // delete target memory block from quick-mem
            _mFreeMemList[freeMemListIndex].next = targetMemBlockLinkPtr->next;
        }

        return targetMemIndex;
    }

    void _quick_mem_memory_merge() {
        for (int i = 0; i < _mFreeMemList.size() - 1; i++) {
            auto firstMemLinkPtr = &(_mFreeMemList[i]);
            __Link *firstMemBlockPtr = firstMemLinkPtr->next;
            __Link *secondMemBlockPtr = nullptr;
            while (firstMemBlockPtr) {
                secondMemBlockPtr = firstMemBlockPtr->next;
                if (SMA_ADDRESS_EQUAL(reinterpret_cast<char *>(firstMemBlockPtr) + (i + 1) * SMA_MEM_ALIGN, secondMemBlockPtr)) {
                    // merge firstMemBlockPtr and secondMemBlockPtr
                    firstMemLinkPtr->next = secondMemBlockPtr->next; // delete first/secondMemBlockPtr from list[i]
                    int listIndex = i * 2 + 1;
                    _insert_mem_block_to_list(firstMemBlockPtr, _INDEX_TO_SIZE(listIndex));
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

                            if (SMA_ADDRESS_EQUAL(addrUP, secondMemBlockPtr) || SMA_ADDRESS_EQUAL(addrDown, secondMemBlockPtr)) { // check merge-able
                                firstMemLinkPtr->next = firstMemBlockPtr->next; // delete firstMemBlock from list[i]
                                secondMemLinkPtr->next = secondMemBlockPtr->next; // delete secondMemBlock from list[j]
                                _insert_mem_block_to_list(
                                    firstMemBlockPtr < secondMemBlockPtr ? firstMemBlockPtr : secondMemBlockPtr,
                                    _INDEX_TO_SIZE(i + j + 1)
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
    }

protected: // memory pool manager
    // manage big-mem-block: bytes > MAX_BLOCK_SIZE
    __MemBlock _mem_pool_allocate(int bytes) {

        __MemBlock targetMemIndex;

        bytes = MEM_ALIGN_ROUND_UP(bytes);

        __Link *preLinkPtr = &(_mFreeMemList[-1]);
        __MemBlock *targetMemBlockPtr = nullptr;
        while (preLinkPtr->next != nullptr) { // FFMA - First Fit
            targetMemBlockPtr = __MemBlock::to_node(preLinkPtr->next);
            if (targetMemBlockPtr->data >= bytes)
                break;
            preLinkPtr = preLinkPtr->next;
        }

        if (preLinkPtr->next != nullptr) {
            // fill mem-block to targetMemIndex
            targetMemIndex.data = targetMemBlockPtr->data;
            targetMemIndex.link.next = __MemBlock::to_link(targetMemBlockPtr);
            // delete memory block from mem-pool
            preLinkPtr->next = targetMemBlockPtr->link.next;
        } else { // allocate failed
            targetMemIndex.link.next = nullptr;
        }

        return targetMemIndex;
    }

    void _mem_pool_memory_merge() {
        __MemBlock *mbPtr = __MemBlock::to_node(_mFreeMemList.back().next);
        while (mbPtr != nullptr) {
            auto next = __MemBlock::to_node(mbPtr->link.next);
            if (SMA_ADDRESS_EQUAL(reinterpret_cast<char *>(mbPtr) + mbPtr->data, next)) {
                // merge next to mbPtr
                mbPtr->data += next->data;
                mbPtr->link.next = next->link.next;
            } else {
                mbPtr = next;
            }
        }
    }

protected: // common
    void _insert_mem_block_to_list(void *addr, int size) {

        if (size <= 0) return;

        int ListIndex = _SIZE_TO_INDEX(size);

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

    void _memory_merge() {
        _quick_mem_memory_merge();
        _mem_pool_memory_merge();
    }

};

}

#endif