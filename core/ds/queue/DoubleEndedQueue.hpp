// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef __DOUBLE_ENDED_QUEUE_HPP__DSTRUCT
#define __DOUBLE_ENDED_QUEUE_HPP__DSTRUCT

#include <core/common.hpp>
#include <core/algorithm.hpp>
#include <core/ds/array/Array.hpp>
#include <core/ds/array/Vector.hpp>

namespace dstruct {

template <typename T, size_t ARR_SIZE, typename Alloc = dstruct::Alloc>
class DoubleEndedQueue;

template <typename T, size_t ARR_SIZE>
class _DoubleEndedQueueIterator : public DStructIteratorTypeSpec<T, RandomIterator> {
    friend class DoubleEndedQueue<T, ARR_SIZE, dstruct::Alloc>;
    friend class _DoubleEndedQueueIterator<const T, ARR_SIZE>; // for it -> const-it
protected:
    using _Array         = dstruct::Array<T, ARR_SIZE>;
    using _ArrMapTable   = dstruct::Vector<_Array *>;
private:
    using __Self = _DoubleEndedQueueIterator;

protected: // only for DoubleEndedQueue
    _DoubleEndedQueueIterator() : _mArrMapTablePtr { nullptr } { }

public:
    _DoubleEndedQueueIterator(
        size_t mapIndex,
        size_t arrIndex,
        _ArrMapTable *mapTablePtr
    ) : _mCurrMapIndex { mapIndex }, _mArrMapTablePtr(mapTablePtr) {
        _mCurr = (*_mArrMapTablePtr)[mapIndex]->begin() + arrIndex;
        __sync();
    }


    _DoubleEndedQueueIterator(const __Self &it) { *this = it; }
    __Self & operator=(const __Self &it) {
        // copy
        _mCurrMapIndex = it._mCurrMapIndex;
        _mCurr = it._mCurr;
        _mArrMapTablePtr = it._mArrMapTablePtr;

        __sync();

        return *this;
    }

    _DoubleEndedQueueIterator(__Self &&it) { *this = dstruct::move(it); }
    __Self & operator=(__Self &&it) {
        // move
        _mCurrMapIndex = it._mCurrMapIndex;
        _mCurr = it._mCurr;
        _mArrMapTablePtr = it._mArrMapTablePtr;

        // reset
        it._mCurrMapIndex = 0;
        it._mCurr = nullptr;
        it._mArrMapTablePtr = nullptr;

        __sync();

        return *this;
    }

    // for it -> const-it
    _DoubleEndedQueueIterator(
        const _DoubleEndedQueueIterator<typename RemoveConst<T>::Type, ARR_SIZE> &obj,
        bool _unsedFlag // constructor dispatch flag
    ) : _DoubleEndedQueueIterator() {

            _unsedFlag;

            _mCurrMapIndex = obj._mCurrMapIndex,
            _mCurr = obj._mCurr.operator->(); // get it-pointer to init const-it
            _mArrMapTablePtr = reinterpret_cast<decltype(_mArrMapTablePtr)>(obj._mArrMapTablePtr);
            __sync();
    }

public: // ForwardIterator
    __Self& operator++() {
        static int cnt = 1;
        _mCurr++;
        if (_mCurr == (*_mArrMapTablePtr)[_mCurrMapIndex]->end()) {
            if (_mCurrMapIndex + 1 < (*_mArrMapTablePtr).capacity()) {
                _mCurr = (*_mArrMapTablePtr)[++_mCurrMapIndex]->begin();
            }
        }
        __sync();
        return *this;
    }

    __Self operator++(int) {
        __Self oldSelf = *this;
        ++(*this);
        return oldSelf;
    }

public: // BidirectionalIterator
    __Self& operator--() {
        if (_mCurr == (*_mArrMapTablePtr)[_mCurrMapIndex]->begin()) {
            _mCurr = (*_mArrMapTablePtr)[--_mCurrMapIndex]->end();
        }
        _mCurr--;
        __sync();
        return *this;
    }

    __Self operator--(int) {
        __Self oldSelf = *this;
        --(*this);
        return oldSelf;
    }

public: // RandomIterator
    __Self operator+(int n) const {
        if (n < 0) {
            return operator-(-n);
        }
        __Self self = *this;
        while (n--) { self++; }
        return self;
    };

    __Self operator-(int n) const {
        if (n < 0) {
            return operator+(-n);
        }
        __Self self = *this;
        while (n--) { self--; }
        return self;
    };

private:
    // update _mLNodePtr and _mPointer
    void __sync() {
        __Self::_mPointer = _mCurr.operator->();
    }

protected:
    size_t _mCurrMapIndex;
    typename _Array::IteratorType _mCurr;     // point to Element / Note: when _mCurr changed, pls __sync
    _ArrMapTable *_mArrMapTablePtr;  // point to Array
};

#define MIN_MAP_TABLE_SIZE 2
 
template <typename T, size_t ARR_SIZE, typename Alloc>
class DoubleEndedQueue :
    public DStructTypeSpec<T, Alloc, _DoubleEndedQueueIterator<T, ARR_SIZE>, _DoubleEndedQueueIterator<const T, ARR_SIZE>> {

protected:
    using _Array         = Array<T, ARR_SIZE>;
    using _AllocArray    = dstruct::AllocSpec<_Array, Alloc>;
    using _ArrMapTable   = Vector<_Array *>;
/*
    struct _Block {
        typename _ArrMapTable::IteratorType arr; // point to Array
        typename _Array::IteratorType curr;      // point to Element
    };
*/

public:
    DoubleEndedQueue() : _mSize { 0 }, _mCapacity { 0 } { }

    DSTRUCT_COPY_SEMANTICS(DoubleEndedQueue) {
        _only_clear();

        for (auto &obj : ds) { // TODO: optimize
            push_back(obj);
        }

        return *this;
    }

    DSTRUCT_MOVE_SEMANTICS(DoubleEndedQueue) {
        _only_clear();

        // move
        _mSize = ds._mSize;
        _mCapacity = ds._mCapacity;
        _mArrMapTable = dstruct::move(ds._mArrMapTable);
        _mBegin = dstruct::move(ds._mBegin);
        _mEnd = dstruct::move(ds._mEnd);

        // reset ds
        ds._mSize = ds._mCapacity = 0;
        ds._mArrMapTable.clear();

        return *this;
    }

    ~DoubleEndedQueue() {
        _only_clear();
    }

public: // base op
    // status
    typename DoubleEndedQueue::SizeType size() const {
        return _mSize;
    }

    typename DoubleEndedQueue::SizeType capacity() const {
        return _mCapacity;
    }

    bool empty() const {
        return _mSize == 0;
    }

public: // check
    typename DoubleEndedQueue::ValueType
    back() const {
        return *(_mEnd - 1);
    }

    typename DoubleEndedQueue::ValueType
    front() const {
        return *(_mBegin._mCurr);
    }

    const T & operator[](int index) const {
        if (index < 0)
            return *(_mEnd + index);
        return *(_mBegin + index);
    }

public: // push/pop
    void push(const T &obj) {
        push_back(obj);
    }

    void push_back(const T &obj) {
        if (_mCapacity == 0) {
            DSTRUCT_ASSERT(_mArrMapTable.empty());
            _only_init();
        } else if (_mEnd._mCurr == _mArrMapTable.back()->end()) {
            _resize(_mCapacity * 2);
        }
        dstruct::construct(&(*_mEnd), obj);
        _mEnd++;
        _mSize++;
    }

    void push_front(const T &obj) {
        if (_mCapacity == 0) {
            DSTRUCT_ASSERT(_mArrMapTable.empty());
            _only_init();
        } else if (_mBegin._mCurr == _mArrMapTable[0]->begin()) {
            _resize(_mCapacity * 2);
        }
        _mBegin--;
        dstruct::construct(_mBegin.operator->(), obj);
        _mSize++;
    }

    void pop() {
        pop_front();
    }

    void pop_back() {
        _mEnd--;
        _mSize--;
        dstruct::destroy(_mEnd.operator->());
        if (_mSize <= _mCapacity / 3 && _mArrMapTable.capacity() > MIN_MAP_TABLE_SIZE) {
            _resize(_mCapacity / 2);
        }
    }

    void pop_front() {
        dstruct::destroy(&(*(_mBegin)));
        _mBegin++;
        _mSize--;
        if (_mSize <= _mCapacity / 3 && _mArrMapTable.capacity() > MIN_MAP_TABLE_SIZE) {
            _resize(_mCapacity / 2);
        }
    }

    void clear() {
        _only_clear();
        //_only_init(); // move to first push
    }

public: // iterator/range-for support

    typename DoubleEndedQueue::ConstIteratorType begin() const {
        return typename DoubleEndedQueue::ConstIteratorType(_mBegin, true);
    }

    typename DoubleEndedQueue::ConstIteratorType end() const {
        return typename DoubleEndedQueue::ConstIteratorType(_mEnd, true);
    }

protected:
    //_Block _mFirst, _mEnd;
    typename DoubleEndedQueue::SizeType _mSize, _mCapacity;
    _ArrMapTable _mArrMapTable;
    typename DoubleEndedQueue::IteratorType _mBegin, _mEnd;

    void _only_clear() {
        if (!_mArrMapTable.empty()) {
            DSTRUCT_ASSERT(_mCapacity != 0);
            // dstruct::destroy element, but don't need to call ~_Array()
            for (auto it = _mBegin; it != _mEnd; it++) {
                dstruct::destroy(it.operator->());
            }

            // release memory
            for (auto arrPtr : _mArrMapTable) {
                _AllocArray::deallocate(arrPtr);
            }

            // reset
            _mSize = _mCapacity = 0;
            _mArrMapTable.clear();
            //_mBegin = _mEnd = nullptr;
        }
    }

    // Note: please obj is null-status before use _only_init
    void _only_init() {

        DSTRUCT_ASSERT(_mArrMapTable.empty() && _mCapacity == 0);

        _mSize = 0;
        _mCapacity = MIN_MAP_TABLE_SIZE * ARR_SIZE;
        _mArrMapTable.resize(MIN_MAP_TABLE_SIZE, nullptr);
        // alloc arr and fill map-table
        for (int i = 0; i < MIN_MAP_TABLE_SIZE; i++) {
            _mArrMapTable[i] = _AllocArray::allocate();
            dstruct::construct(_mArrMapTable[i], _Array());
        }
        auto midMapIndex = MIN_MAP_TABLE_SIZE / 2;
        _mBegin = _mEnd = decltype(_mBegin)(midMapIndex, 0, &_mArrMapTable);

    }

    /* (temp)request1: n % ARR_SIZE == 0 */
    /* request2: _mSize < (n and _mCapacity) */
    // (newCapacity - _mSize) / ARR_SIZE >= 2
    // _mSize = oldCapacity / X
    // newCapacity = oldCapacity / 2
    void _resize(size_t n) {

        bool needUpdateEndCurrIt { false };
        bool isExtend = _mCapacity < n;
        size_t newMapTableSize = n / ARR_SIZE;
        size_t oldArrStartIndex = _mBegin._mCurrMapIndex;
        size_t oldArrEndIndex = _mEnd._mCurrMapIndex;
        size_t arrNum = oldArrEndIndex - oldArrStartIndex;
        size_t newArrStartIndex = ((n - _mSize) / 2) / ARR_SIZE;
        size_t newArrEndIndex = newArrStartIndex + arrNum;

        // request: n / ARR_SIZE <= ((_mSize - 1 - 1) / ARR_SIZE + 1 + 1)
        // n = y * oldCapacity  
        if (!isExtend && newMapTableSize >= arrNum) {
            // when arrNum = ((_mSize - 1 - 1) / ARR_SIZE + 1 + 1)
            // exist newMapTableSize < arrNum
            return;
        }

        _ArrMapTable oldArrMapTable = _mArrMapTable;
        _mArrMapTable.resize(newMapTableSize, nullptr);

        // build new map table
        for (int i = 0; i < newMapTableSize ; i++) {
            if (i < newArrStartIndex || newArrEndIndex < i) {
                _mArrMapTable[i] = _AllocArray::allocate();
                dstruct::construct(_mArrMapTable[i], _Array());
            } else {
                _mArrMapTable[i] = oldArrMapTable[oldArrStartIndex + (i - newArrStartIndex)];
            }
        }

        // release arr in old map table
        for (int i = 0; i < oldArrMapTable.capacity() ; i++) {
            if (i < oldArrStartIndex || oldArrEndIndex < i) {
                //dstruct::destroy(oldArrMapTable[i]);
                _AllocArray::deallocate(oldArrMapTable[i]);
            }
        }

        // update iterator
        _mBegin._mCurrMapIndex = newArrStartIndex;
        _mEnd._mCurrMapIndex = newArrEndIndex;

        // boundary check
        if (_mEnd._mCurr == _mArrMapTable[newArrEndIndex]->end()) {
            _mEnd._mCurrMapIndex++;
            _mEnd._mCurr = _mArrMapTable[_mEnd._mCurrMapIndex]->begin();
            _mEnd.__sync();

            // only if old_mEnd == _mBegin
            if (_mBegin._mCurr == _mArrMapTable[newArrEndIndex]->end()) {
                _mBegin = _mEnd;
            }
        }

        _mCapacity = n;
    }

};

} // namespace dstruct

#endif