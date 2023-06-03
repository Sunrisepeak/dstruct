#ifndef __DOUBLE_ENDED_QUEUE_HPP__DSTRUCT
#define __DOUBLE_ENDED_QUEUE_HPP__DSTRUCT

#include <common.hpp>
#include <ds/static/Array.hpp>
#include <deque>


namespace dstruct {

template<typename T, size_t ARR_SIZE = 32, typename Alloc = port::Alloc>
class DoubleEndedQueue;

template<typename T, size_t ARR_SIZE>
class _DoubleEndedQueueIterator : public DStructIteratorTypeSpec<T, RandomIterator> {
    friend class DoubleEndedQueue<T, ARR_SIZE, port::Alloc>;
protected:
    using _Array         = Array<T, ARR_SIZE>;
    using _ArrMapTable   = Vector<_Array *>;
private:
    using __Self = _DoubleEndedQueueIterator;

public:
    _DoubleEndedQueueIterator(
        typename _Array::IteratorType currIt,
        typename _ArrMapTable::IteratorType currArrIt
    ) : _mCurrArrPtrIt { currArrIt }, _mCurr { currIt } { }

public: // ForwardIterator
    __Self& operator++() {
        _mCurr++;
        if (_mCurr == (*(_mCurrArrPtrIt))->end()) {
            _mCurrArrPtrIt++;
            auto arrPtr = *(_mCurrArrPtrIt);
            if (arrPtr) { // _ArrMapTable will assign a nullptr to "end-element" (N + 1)
                _mCurr = arrPtr->begin();
            }
        }
        return *this;
    }

    __Self operator++(int) {
        __Self oldSelf = *this;
        ++(*this);
        return oldSelf;
    }

public: // BidirectionalIterator
    __Self& operator--() {
        if (_mCurr == (*(_mCurrArrPtrIt))->begin()) {
            _mCurrArrPtrIt--;
            _mCurr = (*(_mCurrArrPtrIt))->end();
        }
        _mCurr--;
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

protected:
    typename _ArrMapTable::IteratorType _mCurrArrPtrIt; // point to Array
    typename _Array::IteratorType _mCurr;               // point to Element
};
 
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
    DoubleEndedQueue() {
        _mArrMapTable.resize(3, nullptr);
        _mArrMapTable[1] =  _AllocArray::allocate();
        _mFirstIt = _mEndIt = decltype(_mFirstIt)(_mArrMapTable[1].begin(), _mArrMapTable.begin() + 1);
        _mSize = 0;
    }



public: // base op
    // status
    typename DoubleEndedQueue::SizeType size() const {
        return _mSize;
    }

    bool empty() const {
        return _mSize == 0;
    }

public: // check
    typename DoubleEndedQueue::ValueType
    back() const {
        return *(_mEndIt._mCurr);
    }

    typename DoubleEndedQueue::ValueType
    front() const {
        return *(_mFirstIt.curr);
    }

public: // push/pop
    void push(const T &obj) {
        push_back(obj);
    }

    void push_back(const T &obj) {
        if (_mEndIt._mCurrArrPtrIt == _mArrMapTable.end()) {
            // resize
        }
        construct(&(*_mEndIt), obj);
        _mEndIt++;
    }

    void push_front(const T &obj) {
        if (
            _mFirstIt._mCurrArrPtrIt == _mArrMapTable.begin() &&
            _mFirstIt._mCurr == (*(_mFirstIt._mCurrArrPtrIt))->begin()
        ) {
            // resize
        }
        _mFirstIt--;
        construct(&(*_mFirstIt), obj);
    }

    void pop() {
        pop_front();
    }

    void pop_back() {
        _mEndIt--;
        _mSize--;
        destory(&(*(_mEndIt)));
        if (_mEndIt._mCurr - _mFirstIt._mCurr < _mArrMapTable.size() / 3) {
            // release
        }
    }

    void pop_front() {
        destory(&(*(_mFirstIt)));
        _mFirstIt++;
        _mSize--;
        if (_mEndIt._mCurr - _mFirstIt._mCurr < _mArrMapTable.size() / 3) {
            // release
        }
    }

    void resize(size_t n) {
        //
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
    _ArrMapTable _mArrMapTable;
    //_Block _mFirst, _mEnd;
    typename DoubleEndedQueue::Iterator _mFirstIt, _mEndIt;
    typename DoubleEndedQueue::SizeType _mSize;

};

} // namespace dstruct

#endif