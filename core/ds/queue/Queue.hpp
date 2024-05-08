// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef QUEUE_HPP_DSTRUCT
#define QUEUE_HPP_DSTRUCT

#include <core/common.hpp>
#include <core/ds/queue/DoubleEndedQueue.hpp>

namespace dstruct {

namespace adapter {

// DS: need support pop_front / push_back 
template <typename T, typename DS>
class Queue {

    DSTRUCT_TYPE_SPEC_HELPER(DS)

public:
    Queue() = default;
    Queue(const Queue &) = default;
    Queue & operator=(const Queue &) = default;
    Queue(Queue &&) = default;
    Queue & operator=(Queue &&) = default;
    ~Queue() = default;

public: // base op
    // status
    SizeType size() const {
        return mDS_d.size();
    }

    //SizeType capacity() const { return mDS_d.capacity(); }

    bool empty() const {
        return mDS_d.empty();
    }

    // check
    ValueType back() const { return mDS_d.back(); }

    ValueType front() const {
        return mDS_d.front();
    }

    const ValueType & operator[](int index) {
        return mDS_d[index];
    }


    // push/pop
    void push(const T &obj) {
        mDS_d.push_back(obj);
    }

    void pop() {
        mDS_d.pop_front();
    }

    // Iterator/range-for
    ConstIteratorType begin() const {
        return mDS_d.begin();
    }

    ConstIteratorType end() const {
        return mDS_d.end();
    }

protected:
    DS mDS_d;
};

}

}

#endif