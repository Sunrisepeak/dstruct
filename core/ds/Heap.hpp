#ifndef __HEAP_HPP__DSTRUCT
#define __HEAP_HPP__DSTRUCT

#include <core/common.hpp>
#include <core/ds/array/Vector.hpp>

namespace dstruct {

// Compare(parent, child) == true
template <typename T, typename Compare, typename Alloc = port::Alloc>
class Heap {

protected:
    using _Heap = dstruct::Vector<T, Alloc>;

    DSTRUCT_TYPE_SPEC_DECLARE(_Heap)

public:
    Heap(const Compare &cmp = Compare(), const T &obj = T()) :
        _mCmp { cmp }, _mHeap(1, obj) { }

    Heap(const IteratorType &begin, const IteratorType &end) : Heap() {

        _mHeap.resize(distance(begin, end) + 1);

        for (auto it = begin; it != end; it++) {
            _mHeap.push_back(*it);
        }

        for (int i = (_mHeap.size() - 1) / 2; i >= 1; i--) {
            _adjust_down(i);
        }
    }

    Heap(Heap &&hep) {
        this->_mCmp = hep._mCmp;
        this->_mHeap = dstruct::move(hep._mHeap);
        hep._mHeap.resize(1, this->_mHeap[0]);
    }

public: // base op
    // status
    SizeType size() const {
        return _mHeap.size() - 1;
    }

    SizeType capacity() const {
        return _mHeap.capacity() - 1;
    }

    bool empty() const {
        return _mHeap.size() == 1;
    }


    // check
    ValueType top() const {
        return _mHeap[1];
    }

    const ValueType & operator[](int index) {
        return _mHeap[index];
    }


    // push/pop
    void push(const T& obj) {
        _mHeap.push_back(obj);
        _adjust_up();
    }

    T pop() {
        T data = _mHeap[1];
        // TODO: undefined or Vector?
        //_mHeap[1] = _mHeap.pop_back(); // when pop_back resize, _mHeap[1] will released
        //_mHeap[1] = dstruct::move(_mHeap.back());
        _mHeap[1] = _mHeap.back();
        _mHeap.pop_back();
        _adjust_down(1);
        return data;
    }


    // iterator/range-for support
    ConstIteratorType begin() const {
        return _mHeap.begin() + 1;
    }

    ConstIteratorType end() const {
        return _mHeap.end();
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
            *it = heap.pop();
        }
    }

protected:
    Compare _mCmp;
    _Heap _mHeap;

    void _adjust_up() {
        int nodeIndex = _mHeap.size() - 1;
        while (nodeIndex > 1) {
            int parent = nodeIndex / 2;
            if (!_mCmp(_mHeap[parent], _mHeap[nodeIndex])) {
                dstruct::swap(_mHeap[parent], _mHeap[nodeIndex]);
                nodeIndex = parent;
            } else {
                break;
            }
        }
    }

    void _adjust_down(int nodeIndex) {
        int heapSize = _mHeap.size();
        int parent = nodeIndex;
        int left, right;

        while (true) {
            int left = 2 * parent;
            int right = left + 1;
            int target = parent;

            if (left < heapSize && !_mCmp(_mHeap[target], _mHeap[left])) {
                target = left;
            }

            if (right < heapSize && !_mCmp(_mHeap[target], _mHeap[right])) {
                target = right;
            }

            if (target == parent) break;

            dstruct::swap(_mHeap[parent], _mHeap[target]);
            parent = target;
        }
    }
};

};

#endif