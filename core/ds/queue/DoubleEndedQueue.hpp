// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef DOUBLE_ENDED_QUEUE_HPP__DSTRUCT
#define DOUBLE_ENDED_QUEUE_HPP__DSTRUCT

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
    using Self = _DoubleEndedQueueIterator;

protected: // only for DoubleEndedQueue
    _DoubleEndedQueueIterator() : mArrMapTablePtr_d { nullptr } { }

public:
    _DoubleEndedQueueIterator(
        size_t mapIndex,
        size_t arrIndex,
        _ArrMapTable *mapTablePtr
    ) : mCurrMapIndex_d { mapIndex }, mArrMapTablePtr_d(mapTablePtr) {
        mCurr_d = (*mArrMapTablePtr_d)[mapIndex]->begin() + arrIndex;
        __sync();
    }


    _DoubleEndedQueueIterator(const Self &it) { *this = it; }
    Self & operator=(const Self &it) {
        // copy
        mCurrMapIndex_d = it.mCurrMapIndex_d;
        mCurr_d = it.mCurr_d;
        mArrMapTablePtr_d = it.mArrMapTablePtr_d;

        __sync();

        return *this;
    }

    _DoubleEndedQueueIterator(Self &&it) { *this = dstruct::move(it); }
    Self & operator=(Self &&it) {
        // move
        mCurrMapIndex_d = it.mCurrMapIndex_d;
        mCurr_d = it.mCurr_d;
        mArrMapTablePtr_d = it.mArrMapTablePtr_d;

        // reset
        it.mCurrMapIndex_d = 0;
        it.mCurr_d = nullptr;
        it.mArrMapTablePtr_d = nullptr;

        __sync();

        return *this;
    }

    // for it -> const-it
    _DoubleEndedQueueIterator(
        const _DoubleEndedQueueIterator<typename RemoveConst<T>::Type, ARR_SIZE> &obj,
        bool _unsedFlag // constructor dispatch flag
    ) : _DoubleEndedQueueIterator() {

            _unsedFlag;

            mCurrMapIndex_d = obj.mCurrMapIndex_d,
            mCurr_d = obj.mCurr_d.operator->(); // get it-pointer to init const-it
            mArrMapTablePtr_d = reinterpret_cast<decltype(mArrMapTablePtr_d)>(obj.mArrMapTablePtr_d);
            __sync();
    }

public: // ForwardIterator
    Self& operator++() {
        static int cnt = 1;
        mCurr_d++;
        if (mCurr_d == (*mArrMapTablePtr_d)[mCurrMapIndex_d]->end()) {
            if (mCurrMapIndex_d + 1 < (*mArrMapTablePtr_d).capacity()) {
                mCurr_d = (*mArrMapTablePtr_d)[++mCurrMapIndex_d]->begin();
            }
        }
        __sync();
        return *this;
    }

    Self operator++(int) {
        Self oldSelf = *this;
        ++(*this);
        return oldSelf;
    }

public: // BidirectionalIterator
    Self& operator--() {
        if (mCurr_d == (*mArrMapTablePtr_d)[mCurrMapIndex_d]->begin()) {
            mCurr_d = (*mArrMapTablePtr_d)[--mCurrMapIndex_d]->end();
        }
        mCurr_d--;
        __sync();
        return *this;
    }

    Self operator--(int) {
        Self oldSelf = *this;
        --(*this);
        return oldSelf;
    }

public: // RandomIterator
    Self operator+(int n) const {
        if (n < 0) {
            return operator-(-n);
        }
        Self self = *this;
        while (n--) { self++; }
        return self;
    };

    Self operator-(int n) const {
        if (n < 0) {
            return operator+(-n);
        }
        Self self = *this;
        while (n--) { self--; }
        return self;
    };

private:
    // update mLNodePtr_d and mPointer_d
    void __sync() {
        Self::mPointer_d = mCurr_d.operator->();
    }

protected:
    size_t mCurrMapIndex_d;
    typename _Array::IteratorType mCurr_d;     // point to Element / Note: when mCurr_d changed, pls __sync
    _ArrMapTable *mArrMapTablePtr_d;  // point to Array
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
    DoubleEndedQueue() : mSize_d { 0 }, mCapacity_d { 0 } { }

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
        mSize_d = ds.mSize_d;
        mCapacity_d = ds.mCapacity_d;
        mArrMapTable_d = dstruct::move(ds.mArrMapTable_d);
        mBegin_d = dstruct::move(ds.mBegin_d);
        mEnd_d = dstruct::move(ds.mEnd_d);

        // reset ds
        ds.mSize_d = ds.mCapacity_d = 0;
        ds.mArrMapTable_d.clear();

        return *this;
    }

    ~DoubleEndedQueue() {
        _only_clear();
    }

public: // base op
    // status
    typename DoubleEndedQueue::SizeType size() const {
        return mSize_d;
    }

    typename DoubleEndedQueue::SizeType capacity() const {
        return mCapacity_d;
    }

    bool empty() const {
        return mSize_d == 0;
    }

public: // check
    typename DoubleEndedQueue::ValueType
    back() const {
        return *(mEnd_d - 1);
    }

    typename DoubleEndedQueue::ValueType
    front() const {
        return *(mBegin_d.mCurr_d);
    }

    const T & operator[](int index) const {
        if (index < 0)
            return *(mEnd_d + index);
        return *(mBegin_d + index);
    }

public: // push/pop
    void push(const T &obj) {
        push_back(obj);
    }

    void push_back(const T &obj) {
        if (mCapacity_d == 0) {
            DSTRUCT_ASSERT(mArrMapTable_d.empty());
            _only_init();
        } else if (mEnd_d.mCurr_d == mArrMapTable_d.back()->end()) {
            _resize(mCapacity_d * 2);
        }
        dstruct::construct(&(*mEnd_d), obj);
        mEnd_d++;
        mSize_d++;
    }

    void push_front(const T &obj) {
        if (mCapacity_d == 0) {
            DSTRUCT_ASSERT(mArrMapTable_d.empty());
            _only_init();
        } else if (mBegin_d.mCurr_d == mArrMapTable_d[0]->begin()) {
            _resize(mCapacity_d * 2);
        }
        mBegin_d--;
        dstruct::construct(mBegin_d.operator->(), obj);
        mSize_d++;
    }

    void pop() {
        pop_front();
    }

    void pop_back() {
        mEnd_d--;
        mSize_d--;
        dstruct::destroy(mEnd_d.operator->());
        if (mSize_d <= mCapacity_d / 3 && mArrMapTable_d.capacity() > MIN_MAP_TABLE_SIZE) {
            _resize(mCapacity_d / 2);
        }
    }

    void pop_front() {
        dstruct::destroy(&(*(mBegin_d)));
        mBegin_d++;
        mSize_d--;
        if (mSize_d <= mCapacity_d / 3 && mArrMapTable_d.capacity() > MIN_MAP_TABLE_SIZE) {
            _resize(mCapacity_d / 2);
        }
    }

    void clear() {
        _only_clear();
        //_only_init(); // move to first push
    }

public: // iterator/range-for support

    typename DoubleEndedQueue::ConstIteratorType begin() const {
        return typename DoubleEndedQueue::ConstIteratorType(mBegin_d, true);
    }

    typename DoubleEndedQueue::ConstIteratorType end() const {
        return typename DoubleEndedQueue::ConstIteratorType(mEnd_d, true);
    }

protected:
    //_Block mFirst_d, mEnd_d;
    typename DoubleEndedQueue::SizeType mSize_d, mCapacity_d;
    _ArrMapTable mArrMapTable_d;
    typename DoubleEndedQueue::IteratorType mBegin_d, mEnd_d;

    void _only_clear() {
        if (!mArrMapTable_d.empty()) {
            DSTRUCT_ASSERT(mCapacity_d != 0);
            // dstruct::destroy element, but don't need to call ~_Array()
            for (auto it = mBegin_d; it != mEnd_d; it++) {
                dstruct::destroy(it.operator->());
            }

            // release memory
            for (auto arrPtr : mArrMapTable_d) {
                _AllocArray::deallocate(arrPtr);
            }

            // reset
            mSize_d = mCapacity_d = 0;
            mArrMapTable_d.clear();
            //mBegin_d = mEnd_d = nullptr;
        }
    }

    // Note: please obj is null-status before use _only_init
    void _only_init() {

        DSTRUCT_ASSERT(mArrMapTable_d.empty() && mCapacity_d == 0);

        mSize_d = 0;
        mCapacity_d = MIN_MAP_TABLE_SIZE * ARR_SIZE;
        mArrMapTable_d.resize(MIN_MAP_TABLE_SIZE, nullptr);
        // alloc arr and fill map-table
        for (int i = 0; i < MIN_MAP_TABLE_SIZE; i++) {
            mArrMapTable_d[i] = _AllocArray::allocate();
            dstruct::construct(mArrMapTable_d[i], _Array());
        }
        auto midMapIndex = MIN_MAP_TABLE_SIZE / 2;
        mBegin_d = mEnd_d = decltype(mBegin_d)(midMapIndex, 0, &mArrMapTable_d);

    }

    /* (temp)request1: n % ARR_SIZE == 0 */
    /* request2: mSize_d < (n and mCapacity_d) */
    // (newCapacity - mSize_d) / ARR_SIZE >= 2
    // mSize_d = oldCapacity / X
    // newCapacity = oldCapacity / 2
    void _resize(size_t n) {

        bool needUpdateEndCurrIt { false };
        bool isExtend = mCapacity_d < n;
        size_t newMapTableSize = n / ARR_SIZE;
        size_t oldArrStartIndex = mBegin_d.mCurrMapIndex_d;
        size_t oldArrEndIndex = mEnd_d.mCurrMapIndex_d;
        size_t arrNum = oldArrEndIndex - oldArrStartIndex;
        size_t newArrStartIndex = ((n - mSize_d) / 2) / ARR_SIZE;
        size_t newArrEndIndex = newArrStartIndex + arrNum;

        // request: n / ARR_SIZE <= ((mSize_d - 1 - 1) / ARR_SIZE + 1 + 1)
        // n = y * oldCapacity  
        if (!isExtend && newMapTableSize >= arrNum) {
            // when arrNum = ((mSize_d - 1 - 1) / ARR_SIZE + 1 + 1)
            // exist newMapTableSize < arrNum
            return;
        }

        _ArrMapTable oldArrMapTable = mArrMapTable_d;
        mArrMapTable_d.resize(newMapTableSize, nullptr);

        // build new map table
        for (int i = 0; i < newMapTableSize ; i++) {
            if (i < newArrStartIndex || newArrEndIndex < i) {
                mArrMapTable_d[i] = _AllocArray::allocate();
                dstruct::construct(mArrMapTable_d[i], _Array());
            } else {
                mArrMapTable_d[i] = oldArrMapTable[oldArrStartIndex + (i - newArrStartIndex)];
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
        mBegin_d.mCurrMapIndex_d = newArrStartIndex;
        mEnd_d.mCurrMapIndex_d = newArrEndIndex;

        // boundary check
        if (mEnd_d.mCurr_d == mArrMapTable_d[newArrEndIndex]->end()) {
            mEnd_d.mCurrMapIndex_d++;
            mEnd_d.mCurr_d = mArrMapTable_d[mEnd_d.mCurrMapIndex_d]->begin();
            mEnd_d.__sync();

            // only if old_mEnd == mBegin_d
            if (mBegin_d.mCurr_d == mArrMapTable_d[newArrEndIndex]->end()) {
                mBegin_d = mEnd_d;
            }
        }

        mCapacity_d = n;
    }

};

} // namespace dstruct

#endif