// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef STACK_HPP_DSTRUCT
#define STACK_HPP_DSTRUCT

#include <core/common.hpp>
#include <core/ds/array/Vector.hpp>

namespace dstruct {

namespace adapter {

// DS: need support pop_back / push_back / back
template <typename T, typename DS>
class Stack {

    DSTRUCT_TYPE_SPEC_HELPER(DS)

public:
    Stack() = default;
    Stack(const Stack &) = default;
    Stack & operator=(const Stack &) = default;
    Stack(Stack &&) = default;
    Stack & operator=(Stack &&) = default;
    ~Stack() = default;

public: // base op
    // status
    SizeType size() const { return mDS_d.size(); }
    //SizeType capacity() const { return mDS_d.capacity(); }
    bool empty() const { return mDS_d.empty(); }

    // check
    ValueType top /* back */ () const { return mDS_d.back(); }

    // push/pop
    void push(const T &obj) { mDS_d.push_back(obj); }
    void pop() {
        T data = mDS_d.back();
        mDS_d.pop_back();
    }

    void clear() {
        mDS_d.clear();
    }

    // Iterator/range-for
    ConstIteratorType begin() const { return mDS_d.begin(); }
    ConstIteratorType end() const { return mDS_d.end(); }

protected:
    DS mDS_d;
};

} // namespace adapter

} // 

#endif