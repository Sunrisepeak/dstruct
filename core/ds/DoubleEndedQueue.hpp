#ifndef __DOUBLE_ENDED_QUEUE_HPP__DSTRUCT
#define __DOUBLE_ENDED_QUEUE_HPP__DSTRUCT

#include <common.hpp>
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
    _DoubleEndedQueueIterator() = default;

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
        _mCapacity = 3;
        _mSize = 0;
        _mArrMapTable.resize(_mCapacity + 1, nullptr);
        // alloc arr and fill map-table
        for (int i = 0; i < _mCapacity - 1 /* save end-nullptr */; i++) {
            _mArrMapTable[i] = _AllocArray::allocate();
            construct(_mArrMapTable[i], _Array());
        }
        construct(_mArrMapTable[1], _Array());
        auto midMapIt = _mArrMapTable.begin() + 1;
        _mBegin = _mEnd = decltype(_mBegin)((*midMapIt)->begin(), midMapIt);
        _mCapacity *= ARR_SIZE;
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
        if (*(_mEnd._mCurrArrPtrIt) == nullptr) {
            _resize(_mSize * 2);
        }
        construct(&(*_mEnd), obj);
        _mEnd++;
        _mSize++;
    }

    void push_front(const T &obj) {
        if (
            _mBegin._mCurrArrPtrIt == _mArrMapTable.begin() &&
            _mBegin._mCurr == (*(_mBegin._mCurrArrPtrIt))->begin()
        ) {
            _resize(_mSize * 2);
        }
        _mBegin--;
        construct(&(*_mBegin), obj);
        _mSize++;
    }

    void pop() {
        pop_front();
    }

    void pop_back() {
        _mEnd--;
        _mSize--;
        destory(&(*(_mEnd)));
        if (_mSize < _mCapacity / 3) {
            _resize(_mCapacity / 2);
        }
    }

    void pop_front() {
        destory(&(*(_mBegin)));
        _mBegin++;
        _mSize--;
        if (_mSize < _mCapacity / 3) {
            _resize(_mCapacity / 2);
        }
    }

    void resize(size_t n, const T &obj) {
        // release
        for (auto &arrPtr : _mArrMapTable) {
            if (arrPtr) {
                destory(arrPtr);
                arrPtr = nullptr;
            }
        }
        // compute size
        size_t mapTabSize = (n / ARR_SIZE) + 2 + 1;
        _mArrMapTable.resize(mapTabSize, nullptr);
        // alloc arr and fill map-table
        for (int i = 0; i < _mArrMapTable.capacity() - 1 /* save end-nullptr */; i++) {
            _mArrMapTable[i] = _AllocArray::allocate();
            construct(_mArrMapTable[i], _Array());
        }
        // init obj
        _mBegin._mCurrArrPtrIt = _mEnd._mCurrArrPtrIt = _mArrMapTable.begin() + 1;
        _mBegin._mCurr = _mEnd._mCurr = (*(_mBegin._mCurrArrPtrIt))->begin();
        for (int i = 0; i < n; i++) {
            // construct(addr, obj)
            *(++_mEnd) = obj;
        }
        _mCapacity = _mSize = n;
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
    typename DoubleEndedQueue::IteratorType _mBegin, _mEnd;
    typename DoubleEndedQueue::SizeType _mSize, _mCapacity;


    void _resize(size_t n) {
        auto oldStartIndex = distance(_mArrMapTable.begin(), _mBegin._mCurrArrPtrIt);
        auto oldEndIndex = distance(_mArrMapTable.begin(), _mEnd._mCurrArrPtrIt);
        _ArrMapTable old = _mArrMapTable;
        _mArrMapTable.resize(n, nullptr);
        size_t diff = n > old.capacity() ? n - old.capacity() : old.capacity() - n;
        size_t startIndex = diff / 2;
        size_t endIndex = _mArrMapTable.capacity() - diff / 2 - 1;
        for (int i = (_mArrMapTable.capacity() - 1) - 1; i >= 0 ; i--) {
            if (startIndex > i && i > endIndex) {
                _mArrMapTable[i] = _AllocArray::allocate();
                construct(_mArrMapTable[i], _Array());
            } else {
                if (!old.empty()) {
                    _mArrMapTable[i] = old.back();
                    old.pop_back();
                }
            }
        }
        _mBegin._mCurrArrPtrIt = (_mArrMapTable.begin() + startIndex) + oldStartIndex;
        _mEnd._mCurrArrPtrIt = (_mArrMapTable.begin() + startIndex) + oldEndIndex;
        _mCapacity = n;
    }

};

} // namespace dstruct

#endif