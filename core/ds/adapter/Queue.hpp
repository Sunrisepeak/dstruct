#ifndef __QUEUE_HPP__DSTRUCT
#define __QUEUE_HPP__DSTRUCT

#include <common.hpp>
#include <ds/Queue/DoubleEndedQueue.hpp>

namespace dstruct {

namespace adapter {

// DS: need support pop_front / push_back 
template<typename T, typename DS>
class Queue : public _DStructTypeSpec<T, typename DS::IteratorType, typename DS::ConstIteratorType> {

    DSTRUCT_TYPE_SPEC_DECLARE(DS)

public:
    Queue() = default;

public: // base op
    // status
    SizeType size() const {
        return _mDS.size();
    }

    //SizeType capacity() const { return _mDS.capacity(); }

    bool empty() const {
        return _mDS.empty();
    }

    // check
    //ValueType back() const { return _mDS.back(); }

    ValueType front() const {
        return _mDS.front();
    }

    const ValueType & operator[](int index) {
        return _mDS[index];
    }


    // push/pop
    void push(const T &obj) {
        _mDS.push_back(obj);
    }

    void pop() {
        _mDS.pop_front();
    }

    // Iterator/range-for
    ConstIteratorType begin() const {
        return _mDS.begin();
    }

    ConstIteratorType end() const {
        return _mDS.end();
    }

protected:
    DS _mDS;
};

}

}

#endif