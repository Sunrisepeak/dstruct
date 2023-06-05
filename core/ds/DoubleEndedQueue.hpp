#ifndef __DOUBLE_ENDED_QUEUE_HPP__DSTRUCT
#define __DOUBLE_ENDED_QUEUE_HPP__DSTRUCT

#include <common.hpp>
#include <algorithm.hpp>
#include <ds/static/Array.hpp>

namespace dstruct {

template<typename T, size_t ARR_SIZE, typename Alloc = port::Alloc>
class DoubleEndedQueue;

template<typename T, size_t ARR_SIZE>
class _DoubleEndedQueueIterator : public DStructIteratorTypeSpec<T, RandomIterator> {
    friend class DoubleEndedQueue<T, ARR_SIZE, port::Alloc>;
protected:
    using _Array         = Array<T, ARR_SIZE>;
    using _ArrMapTable   = Vector<_Array *>;
private:
    using __Self = _DoubleEndedQueueIterator;

protected: // only for DoubleEndedQueue
    _DoubleEndedQueueIterator() : _mArrMapTablePtr { nullptr } { };

public:
    _DoubleEndedQueueIterator(
        size_t arrIndex,
        typename _Array::IteratorType currIt,
        _ArrMapTable *mapTablePtr
    ) : _mCurrArrIndex { arrIndex }, _mCurr { currIt }, _mArrMapTablePtr(mapTablePtr) {
        __sync();
    }
/*
    _DoubleEndedQueueIterator(const __Self&) = default;
    _DoubleEndedQueueIterator(__Self&&) = default;
    __Self & operator=(const __Self&) = default;
    __Self & operator=(__Self&&) = default;
*/

public: // ForwardIterator
    __Self& operator++() {
        static int cnt = 1;
        _mCurr++;
        if (_mCurr == (*_mArrMapTablePtr)[_mCurrArrIndex]->end()) {
            if (_mCurrArrIndex + 1 < (*_mArrMapTablePtr).capacity()) {
                _mCurr = (*_mArrMapTablePtr)[++_mCurrArrIndex]->begin();
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
        if (_mCurr == (*_mArrMapTablePtr)[_mCurrArrIndex]->begin()) {
            _mCurr = (*_mArrMapTablePtr)[--_mCurrArrIndex]->end();
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
/*
public: // RandomIterator
    __Self operator+(const int &n) const { return __Self::mPointer + n; };
    __Self operator-(const int &n) const { return __Self::mPointer -n; };
    typename __Self::ReferenceType operator[](int index) { return __Self::mPointer[index]; }
    typename __Self::ValueType operator[](int index) const { return __Self::mPointer[index]; };
*/

private:
    // update _mLNodePtr and mPointer
    void __sync() {
        __Self::mPointer = _mCurr.operator->();
    }

protected:
    size_t _mCurrArrIndex;
    typename _Array::IteratorType _mCurr;     // point to Element / Note: when _mCurr changed, pls __sync
    _ArrMapTable *_mArrMapTablePtr;  // point to Array
};

#define MIN_MAP_TABLE_SIZE 2
 
template<typename T, size_t ARR_SIZE, typename Alloc>
class DoubleEndedQueue :
    public _DStructTypeSpec<T, Alloc, _DoubleEndedQueueIterator<T, ARR_SIZE>, _DoubleEndedQueueIterator<const T, ARR_SIZE>> {

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
        _mBegin = _mEnd = decltype(_mBegin)(midMapIndex, _mArrMapTable[midMapIndex]->begin(), &_mArrMapTable);
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
        return *(_mEnd._mCurr);
    }

    typename DoubleEndedQueue::ValueType
    front() const {
        return *(_mBegin.curr);
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
        destory(&(*(_mEnd)));
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

/*
public: // iterator/range-for support
    IteratorType begin() {

    }

    ConstIteratorType begin() const {

    }

    IteratorType end() {

    }

    ConstIteratorType end() const {

    }
*/

protected:
    //_Block _mFirst, _mEnd;
    typename DoubleEndedQueue::SizeType _mSize, _mCapacity;
    _ArrMapTable _mArrMapTable;
    typename DoubleEndedQueue::IteratorType _mBegin, _mEnd;

    /* (temp)request1: n % ARR_SIZE == 0 */
    /* request2: _mSize < (n and _mCapacity) */
    void _resize(size_t n) {

        bool needUpdateEndCurrIt { false };
        bool isExtend = _mCapacity < n;
        size_t newMapTableSize = n / ARR_SIZE;
        size_t oldArrStartIndex = _mBegin._mCurrArrIndex;
        size_t oldArrEndIndex = _mEnd._mCurrArrIndex;
        size_t newArrStartIndex = ((n - _mSize) / 2) / ARR_SIZE;
        size_t newArrEndIndex = newArrStartIndex + (oldArrEndIndex - oldArrStartIndex);

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
                destory(oldArrMapTable[i]);
                _AllocArray::deallocate(oldArrMapTable[i]);
            }
        }

        // update iterator
        _mBegin._mCurrArrIndex = newArrStartIndex;

        if (_mEnd._mCurr == _mArrMapTable[newArrEndIndex]->end()) {
            newArrEndIndex++;
            _mEnd._mCurrArrIndex = newArrEndIndex;
            _mEnd._mCurr = _mArrMapTable[newArrEndIndex]->begin();
            _mEnd.__sync();
        } else {
            _mEnd._mCurrArrIndex = newArrEndIndex;
        }

        _mCapacity = n;
    }

};

} // namespace dstruct

#endif