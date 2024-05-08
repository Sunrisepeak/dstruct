// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef STATIC_MEM_ALLOCATOR_HPP_DSTRUCT
#define STATIC_MEM_ALLOCATOR_HPP_DSTRUCT

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
mFreeMemList_d: when SMA_MEM_ALIGN equal 8
    +---------------------------+
    | 0 | 1 | ......... |n-1| X | - index
    +---------------------------+
    | l | l | ......... | l | p | - list-head 
    ----------------------------+
      |                  |     |
      V                  V     V 
    list                list  list-mem-pool            - list
    Link_e             Link_e  MemBlock_e              - list-node
    8*(0+1)             8*n    MemBlock_e->data        - mem-block size
*/

// Note: request MEMORY_SIZE % SMA_MEM_ALIGN == 0
template <int MEMORY_SIZE, int MAX_BLOCK_SIZE = 128>
struct StaticMemAllocator {

private:
    using Link_e = SinglyLink_;
    /*
        struct MemBlock_e {
            Link_e link;
            int data;
        }
    */
    using MemBlock_e = dstruct::EmbeddedListNode_<int, Link_e>;

private: // big five
    StaticMemAllocator() : mFreeMemSize_d { MEMORY_SIZE }, mMemoryPool_d { 0 }, mFreeMemList_d(Link_e{ nullptr }) {
        auto mbPtr = reinterpret_cast<MemBlock_e *>(&mMemoryPool_d);
        mbPtr->data = MEMORY_SIZE;
        mbPtr->link.next = nullptr;
        mFreeMemList_d[-1].next = MemBlock_e::to_link(mbPtr);
    }

    StaticMemAllocator(const StaticMemAllocator&) = delete;
    StaticMemAllocator & operator=(const StaticMemAllocator&) = delete;

public: // mem-alloc interface
    static void * allocate(int bytes) {
        return Instance_().allocate_d(bytes);
    }

    static bool deallocate(void *addr, int bytes) {
        return Instance_().deallocate_d(addr, bytes);
    }

public: // mem-manager interface

    constexpr static int MEM_ALIGN_ROUND_UP(int bytes) {
        return (((bytes) + SMA_MEM_ALIGN - 1) & ~(SMA_MEM_ALIGN - 1));
    }

    static int free_mem_size() {
        return Instance_().mFreeMemSize_d;
    }

    static int max_free_mblock_size() {
        // check - mem-pool
        auto  *linkPtr = Instance_().mFreeMemList_d.back().next;
        int maxSize = 0;
        while (linkPtr != nullptr) {
            auto mbPtr = MemBlock_e::to_node(linkPtr);
            if (maxSize < mbPtr->data) maxSize = mbPtr->data;
            linkPtr = linkPtr->next;
        }

        if (maxSize > 0) return maxSize;

        // check - quick-mem
        int listIndex = Instance_().mFreeMemList_d.size() - 1 - 1;
        while (listIndex >= 0 && Instance_().mFreeMemList_d[listIndex].next == nullptr) {
            listIndex--;
        }

        maxSize = INDEX_TO_SIZE_(listIndex);

        return maxSize;
    }

    static void memory_merge() {
        Instance_().memory_merge_d();
    }

    static void dump() {
        SMA_LOGD("sma dump(total %d, used %d, free %d):",
            MEMORY_SIZE, MEMORY_SIZE - Instance_().mFreeMemSize_d, Instance_().mFreeMemSize_d);
        int verifyFreeMemSize = 0;
        for (int i = 0; i < Instance_().mFreeMemList_d.size(); i++) {
            Link_e *mbPtr = Instance_().mFreeMemList_d[i].next;
            int lIndex = 0;
            while (mbPtr != nullptr) {
                int size = 0;
                if (i == Instance_().mFreeMemList_d.size() - 1) {
                    size = MemBlock_e::to_node(mbPtr)->data;
                } else {
                    size = INDEX_TO_SIZE_(i);
                }
                SMA_LOGD("\tt-index: %d, l-index %d, addr %p, size %d", i, lIndex++, mbPtr, size);
                verifyFreeMemSize += size;
                mbPtr = mbPtr->next;
            }
        }

        SMA_LOGD("\tfree-mem verify: %d == %d", Instance_().mFreeMemSize_d, verifyFreeMemSize);

        DSTRUCT_CRASH(Instance_().mFreeMemSize_d != verifyFreeMemSize);
    }

protected:
    int mFreeMemSize_d;
    char mMemoryPool_d[MEMORY_SIZE];
    dstruct::Array<Link_e, MAX_BLOCK_SIZE / SMA_MEM_ALIGN + 1/*MemPoolManager*/> mFreeMemList_d;

    static StaticMemAllocator & Instance_() {
        static StaticMemAllocator sma; // create & manage static memory area
        return sma;
    }

    constexpr static int SIZE_TO_INDEX_(int bytes) {
        return MEM_ALIGN_ROUND_UP(bytes) / SMA_MEM_ALIGN - 1;
    }

    constexpr static int INDEX_TO_SIZE_(int index) {
        return (index + 1) * SMA_MEM_ALIGN;
    }

    void * allocate_d(int bytes) {

        MemBlock_e targetMemBlockIndex;

        if (bytes > MAX_BLOCK_SIZE) {
            targetMemBlockIndex = mem_pool_allocate_d(bytes);
        } else {
            targetMemBlockIndex = quick_mem_allocate_d(bytes);
            if (targetMemBlockIndex.link.next == nullptr)
                targetMemBlockIndex = mem_pool_allocate_d(bytes);
        }

        Link_e *memPtr = targetMemBlockIndex.link.next;

        if (memPtr == nullptr) return nullptr;

        int allocatedSize = MEM_ALIGN_ROUND_UP(bytes);
        void *memFragmentAddr = reinterpret_cast<char *>(memPtr) + allocatedSize;
        int memFragmentSize = targetMemBlockIndex.data - allocatedSize;

        // insert memory fragment to list
        insert_mem_block_to_list_d(memFragmentAddr, memFragmentSize);

        mFreeMemSize_d -= allocatedSize;

        return memPtr;
    }

    bool deallocate_d(void *addr, int bytes) {
        if (bytes <= 0 || addr == nullptr) return false;
        insert_mem_block_to_list_d(addr, MEM_ALIGN_ROUND_UP(bytes));
        mFreeMemSize_d += MEM_ALIGN_ROUND_UP(bytes);
        return true;
    }

protected: // quick allocator
    // request: bytes <= MAX_BLOCK_SIZE
    MemBlock_e quick_mem_allocate_d(int bytes) {

        MemBlock_e targetMemIndex;

        bytes = MEM_ALIGN_ROUND_UP(bytes);

        int freeMemListIndex = SIZE_TO_INDEX_(bytes);
        while ( // search free memory block
            freeMemListIndex < mFreeMemList_d.size() - 1 &&
            mFreeMemList_d[freeMemListIndex].next == nullptr
        ) freeMemListIndex++;

        if (freeMemListIndex == mFreeMemList_d.size() - 1) {
            targetMemIndex.link.next = nullptr;
            targetMemIndex.data = 0;
        } else {
            // fill mem block info(addr and size) to targetMemIndex
            auto targetMemBlockLinkPtr = mFreeMemList_d[freeMemListIndex].next;
            targetMemIndex.link.next = targetMemBlockLinkPtr;
            targetMemIndex.data = INDEX_TO_SIZE_(freeMemListIndex);
            // delete target memory block from quick-mem
            mFreeMemList_d[freeMemListIndex].next = targetMemBlockLinkPtr->next;
        }

        return targetMemIndex;
    }

    void quick_mem_memory_merge_d() {
        for (int i = 0; i < mFreeMemList_d.size() - 1; i++) {
            auto firstMemLinkPtr = &(mFreeMemList_d[i]);
            Link_e *firstMemBlockPtr = firstMemLinkPtr->next;
            Link_e *secondMemBlockPtr = nullptr;
            while (firstMemBlockPtr) {
                secondMemBlockPtr = firstMemBlockPtr->next;
                if (SMA_ADDRESS_EQUAL(reinterpret_cast<char *>(firstMemBlockPtr) + (i + 1) * SMA_MEM_ALIGN, secondMemBlockPtr)) {
                    // merge firstMemBlockPtr and secondMemBlockPtr
                    firstMemLinkPtr->next = secondMemBlockPtr->next; // delete first/secondMemBlockPtr from list[i]
                    int listIndex = i * 2 + 1;
                    insert_mem_block_to_list_d(firstMemBlockPtr, INDEX_TO_SIZE_(listIndex));
                } else {
                    bool secondMemBlockFlag = false;
                    for (int j = i + 1; j < mFreeMemList_d.size() - 1 && !secondMemBlockFlag; j++) {
                        auto secondMemLinkPtr = &(mFreeMemList_d[j]);
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
                                insert_mem_block_to_list_d(
                                    firstMemBlockPtr < secondMemBlockPtr ? firstMemBlockPtr : secondMemBlockPtr,
                                    INDEX_TO_SIZE_(i + j + 1)
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
    MemBlock_e mem_pool_allocate_d(int bytes) {

        MemBlock_e targetMemIndex;

        bytes = MEM_ALIGN_ROUND_UP(bytes);

        Link_e *preLinkPtr = &(mFreeMemList_d[-1]);
        MemBlock_e *targetMemBlockPtr = nullptr;
        while (preLinkPtr->next != nullptr) { // FFMA - First Fit
            targetMemBlockPtr = MemBlock_e::to_node(preLinkPtr->next);
            if (targetMemBlockPtr->data >= bytes)
                break;
            preLinkPtr = preLinkPtr->next;
        }

        if (preLinkPtr->next != nullptr) {
            // fill mem-block to targetMemIndex
            targetMemIndex.data = targetMemBlockPtr->data;
            targetMemIndex.link.next = MemBlock_e::to_link(targetMemBlockPtr);
            // delete memory block from mem-pool
            preLinkPtr->next = targetMemBlockPtr->link.next;
        } else { // allocate failed
            targetMemIndex.link.next = nullptr;
        }

        return targetMemIndex;
    }

    void mem_pool_memory_merge_d() {
        MemBlock_e *mbPtr = MemBlock_e::to_node(mFreeMemList_d.back().next);
        while (mbPtr != nullptr) {
            auto next = MemBlock_e::to_node(mbPtr->link.next);
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
    void insert_mem_block_to_list_d(void *addr, int size) {

        if (size <= 0) return;

        int ListIndex = SIZE_TO_INDEX_(size);

        if (ListIndex >= mFreeMemList_d.size() - 1) {
            ListIndex = mFreeMemList_d.size() - 1;
        }

        Link_e *freeMemLinkPtr = &(mFreeMemList_d[ListIndex]);
        while (freeMemLinkPtr->next != nullptr && addr > freeMemLinkPtr->next) {
            freeMemLinkPtr = freeMemLinkPtr->next;
        }

        Link_e::add(freeMemLinkPtr, static_cast<Link_e *>(addr));

        if (ListIndex == mFreeMemList_d.size() - 1) {
            // insert to MemoryPool, so need to set mem-block size
            MemBlock_e *mbPtr = MemBlock_e::to_node(freeMemLinkPtr->next);
            mbPtr->data = size;
        }

    }

    void memory_merge_d() {
        quick_mem_memory_merge_d();
        mem_pool_memory_merge_d();
    }

};

}

#endif