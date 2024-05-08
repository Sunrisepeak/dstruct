// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef HEAP_HPP_DSTRUCT
#define HEAP_HPP_DSTRUCT

#include <core/common.hpp>
#include <core/ds/array/Vector.hpp>

namespace dstruct {

// Compare(parent, child) == true
template <typename T, typename Compare, typename Alloc = dstruct::Alloc>
class Heap {

protected:
    using Heap_ = dstruct::Vector<T, Alloc>;

    DSTRUCT_TYPE_SPEC_HELPER(Heap_)

public:
    Heap(const Compare &cmp = Compare(), const T &obj = T()) :
        mCmp_d { cmp }, mHeap_d(1, obj) { }

    Heap(const IteratorType &begin, const IteratorType &end) : Heap() {

        mHeap_d.resize(distance(begin, end) + 1);

        for (auto it = begin; it != end; it++) {
            mHeap_d.push_back(*it);
        }

        for (int i = (mHeap_d.size() - 1) / 2; i >= 1; i--) {
            _adjust_down(i);
        }
    }

    Heap(const Heap &hep) { *this = hep; }
    Heap & operator=(const Heap &hep) {
        mCmp_d = hep.mCmp_d;
        mHeap_d = hep.mHeap_d;
        return *this;
    }

    Heap(Heap &&hep) { *this = dstruct::move(hep); }
    Heap & operator=(Heap &&hep) {
        this->mCmp_d = hep.mCmp_d;
        this->mHeap_d = dstruct::move(hep.mHeap_d);
        hep.mHeap_d.resize(1, this->mHeap_d[0]);
        return *this;
    }

    ~Heap() = default;

public: // base op
    // status
    SizeType size() const {
        return mHeap_d.size() - 1;
    }

    SizeType capacity() const {
        return mHeap_d.capacity() - 1;
    }

    bool empty() const {
        return mHeap_d.size() == 1;
    }


    // check
    ValueType top() const {
        return mHeap_d[1];
    }

    const ValueType & operator[](int index) {
        return mHeap_d[index];
    }


    // push/pop
    void push(const T& obj) {
        mHeap_d.push_back(obj);
        _adjust_up();
    }

    void pop() {
        // T data = mHeap_d[1];
        // TODO: undefined or Vector?
        //mHeap_d[1] = mHeap_d.pop_back(); // when pop_back resize, mHeap_d[1] will released
        //mHeap_d[1] = dstruct::move(mHeap_d.back());
        mHeap_d[1] = mHeap_d.back();
        mHeap_d.pop_back();
        _adjust_down(1);
        // return data;
    }


    // iterator/range-for support
    ConstIteratorType begin() const {
        return mHeap_d.begin() + 1;
    }

    ConstIteratorType end() const {
        return mHeap_d.end();
    }

public: // pub static

    static Heap build(const IteratorType &begin, const IteratorType &end) {
        Heap heap(begin, end);
        auto heapIt = heap.begin();
        for (auto it = begin; it != end; it++, heapIt++) {
            *it = *heapIt;
        }
        return heap /*todo: dstruct::move(heap)*/;
    }

    static void sort(const IteratorType &begin, const IteratorType &end) {
        Heap heap(begin, end);
        for (auto it = begin; it != end; it++) {
            *it = heap.top(); heap.pop();
        }
    }

protected:
    Compare mCmp_d;
    Heap_ mHeap_d;

    void _adjust_up() {
        int nodeIndex = mHeap_d.size() - 1;
        while (nodeIndex > 1) {
            int parent = nodeIndex / 2;
            if (!mCmp_d(mHeap_d[parent], mHeap_d[nodeIndex])) {
                dstruct::swap(mHeap_d[parent], mHeap_d[nodeIndex]);
                nodeIndex = parent;
            } else {
                break;
            }
        }
    }

    void _adjust_down(int nodeIndex) {
        int heapSize = mHeap_d.size();
        int parent = nodeIndex;
        int left, right;

        while (true) {
            int left = 2 * parent;
            int right = left + 1;
            int target = parent;

            if (left < heapSize && !mCmp_d(mHeap_d[target], mHeap_d[left])) {
                target = left;
            }

            if (right < heapSize && !mCmp_d(mHeap_d[target], mHeap_d[right])) {
                target = right;
            }

            if (target == parent) break;

            dstruct::swap(mHeap_d[parent], mHeap_d[target]);
            parent = target;
        }
    }
};

};

#endif