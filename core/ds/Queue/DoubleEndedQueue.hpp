#ifndef __DOUBLE_ENDED_QUEUE_HPP__DSTRUCT
#define __DOUBLE_ENDED_QUEUE_HPP__DSTRUCT

#include <core/common.hpp>
#include <core/algorithm.hpp>
#include <core/ds/static/Array.hpp>
#include <core/ds/Vector.hpp>

namespace dstruct {

template<typename T, size_t ARR_SIZE, typename Alloc = port::Alloc>
class DoubleEndedQueue;

template<typename T, size_t ARR_SIZE>
class _DoubleEndedQueueIterator : public DStructIteratorTypeSpec<T, RandomIterator> {
    friend class DoubleEndedQueue<T, ARR_SIZE, port::Alloc>;
    friend class _DoubleEndedQueueIterator<const T, ARR_SIZE>; // for it -> const-it
protected:
    using _Array         = Array<T, ARR_SIZE>;
    using _ArrMapTable   = Vector<_Array *>;
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

/*
    _DoubleEndedQueueIterator(const __Self&) = default;
    _DoubleEndedQueueIterator(__Self&&) = default;
    __Self & operator=(const __Self&) = default;
    __Self & operator=(__Self&&) = default;
*/
    // for it -> const-it
    _DoubleEndedQueueIterator(const _DoubleEndedQueueIterator<typename types::RemoveConst<T>::Type, ARR_SIZE> &obj) :
        _DoubleEndedQueueIterator() {
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
    // update _mLNodePtr and mPointer
    void __sync() {
        __Self::mPointer = _mCurr.operator->();
    }

protected:
    size_t _mCurrMapIndex;
    typename _Array::IteratorType _mCurr;     // point to Element / Note: when _mCurr changed, pls __sync
    _ArrMapTable *_mArrMapTablePtr;  // point to Array
};

#define MIN_MAP_TABLE_SIZE 2
 
template<typename T, size_t ARR_SIZE, typename Alloc>
class DoubleEndedQueue :
    public _DStructTypeSpec<T, _DoubleEndedQueueIterator<T, ARR_SIZE>, _DoubleEndedQueueIterator<const T, ARR_SIZE>, Alloc> {

protected:
    using _Array      = Array<T, ARR_SIZE>;
    using _AllocArray = DSAlloc<_Array, Alloc>;
    using _ArrMapTable   = Vector<_Array *>;
/*
    struct _Block {
        typename _ArrMapTable::IteratorType arr; // point to Array
        typename _Array::IteratorType curr;      // point to Element
    };
*/

public:
    DoubleEndedQueue() :
      _mSize { 0 },
      _mCapacity { MIN_MAP_TABLE_SIZE * ARR_SIZE },
      _mArrMapTable(MIN_MAP_TABLE_SIZE, nullptr),
      _mBegin(),
      _mEnd(_mBegin)
    {
        // alloc arr and fill map-table
        for (int i = 0; i < MIN_MAP_TABLE_SIZE; i++) {
            _mArrMapTable[i] = _AllocArray::allocate();
            construct(_mArrMapTable[i], _Array());
        }
        auto midMapIndex = MIN_MAP_TABLE_SIZE / 2;
        _mBegin = _mEnd = decltype(_mBegin)(midMapIndex, 0, &_mArrMapTable);
    }

    ~DoubleEndedQueue() {
        // destory element, but don't use ~_Array()
        for (auto it = _mBegin; it != _mEnd; it++) {
            destory(it.operator->());
        }

        // release memory
        for (auto arrPtr : _mArrMapTable) {
            _AllocArray::deallocate(arrPtr);
        }
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

    const T& operator[](int index) const {
        if (index < 0)
            return *(_mEnd + index);
        return *(_mBegin + index);
    }

public: // push/pop
    void push(const T &obj) {
        push_back(obj);
    }

    void push_back(const T &obj) {
        if (_mEnd._mCurr == _mArrMapTable.back()->end()) {
            _resize(_mCapacity * 2);
        }
        construct(&(*_mEnd), obj);
        _mEnd++;
        _mSize++;
    }

    void push_front(const T &obj) {
        if (_mBegin._mCurr == _mArrMapTable[0]->begin()) {
            _resize(_mCapacity * 2);
        }
        _mBegin--;
/*
        DSTRUCT_ASSERT(_mBegin._mArrMapTable == _mArrMapTable.begin());
        DSTRUCT_ASSERT(*(_mBegin._mArrMapTable) == _mArrMapTable[0]);
        auto end = ((*(_mArrMapTable.begin()))->end());
        DSTRUCT_ASSERT(_mBegin._mCurr == end - 1);
        DSTRUCT_ASSERT((_mBegin._mCurr).operator->() == end.operator->() - 1);
        DSTRUCT_ASSERT((_mBegin._mCurr).operator->() == _mBegin.operator->());
        DSTRUCT_ASSERT(sizeof(*_mBegin) == sizeof(int));
*/
        construct(_mBegin.operator->(), obj);
        //auto arrPtr = *(_mBegin._mArrMapTable);
        //(*(*(_mBegin._mArrMapTable)))[distance(_mBegin._mCurr, arrPtr->end())] = obj;
        _mSize++;
    }

    void pop() {
        pop_front();
    }

    void pop_back() {
        _mEnd--;
        _mSize--;
        destory(_mEnd.operator->());
        if (_mSize <= _mCapacity / 3 && _mArrMapTable.capacity() > MIN_MAP_TABLE_SIZE) {
            _resize(_mCapacity / 2);
        }
    }

    void pop_front() {
        destory(&(*(_mBegin)));
        _mBegin++;
        _mSize--;
        if (_mSize <= _mCapacity / 3 && _mArrMapTable.capacity() > MIN_MAP_TABLE_SIZE) {
            _resize(_mCapacity / 2);
        }
    }

public: // iterator/range-for support

    typename DoubleEndedQueue::ConstIteratorType begin() const {
        return _mBegin;
    }

    typename DoubleEndedQueue::ConstIteratorType end() const {
        return _mEnd;
    }

protected:
    //_Block _mFirst, _mEnd;
    typename DoubleEndedQueue::SizeType _mSize, _mCapacity;
    _ArrMapTable _mArrMapTable;
    typename DoubleEndedQueue::IteratorType _mBegin, _mEnd;

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
                construct(_mArrMapTable[i], _Array());
            } else {
                _mArrMapTable[i] = oldArrMapTable[oldArrStartIndex + (i - newArrStartIndex)];
            }
        }

        // release arr in old map table
        for (int i = 0; i < oldArrMapTable.capacity() ; i++) {
            if (i < oldArrStartIndex || oldArrEndIndex < i) {
                //destory(oldArrMapTable[i]);
                _AllocArray::deallocate(oldArrMapTable[i]);
            }
        }

        // update iterator
        _mBegin._mCurrMapIndex = newArrStartIndex;

        if (_mEnd._mCurr == _mArrMapTable[newArrEndIndex]->end()) {
            newArrEndIndex++;
            _mEnd._mCurrMapIndex = newArrEndIndex;
            _mEnd._mCurr = _mArrMapTable[newArrEndIndex]->begin();
            _mEnd.__sync();
        } else {
            _mEnd._mCurrMapIndex = newArrEndIndex;
        }

        _mCapacity = n;
    }

};

} // namespace dstruct

#endif