// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef XVALUE_STACK_HPP_DSTRUCT
#define XVALUE_STACK_HPP_DSTRUCT

#include <core/common.hpp>
#include <core/ds/stack/Stack.hpp>
#include <core/ds/array/Vector.hpp>

namespace dstruct {

namespace stack {

template <typename T, typename Compare, typename StackType = adapter::Stack<T, Vector<T>>>
class XValueStack {

    DSTRUCT_TYPE_SPEC_HELPER(StackType)

public:
    XValueStack() = default;
    XValueStack(const Compare &cmp) : mCmp_d { cmp } { }

    DSTRUCT_COPY_SEMANTICS(XValueStack) {
        this->mXValue_d = ds.mXValue_d;
        this->mCmp_d = ds.mCmp_d;
        this->mStack_d = ds.mStack_d;
        return *this;
    }

    DSTRUCT_MOVE_SEMANTICS(XValueStack) {
        mXValue_d = dstruct::move(ds.mXValue_d);
        mCmp_d =  dstruct::move(ds.mCmp_d);
        mStack_d =  dstruct::move(ds.mStack_d);
        return *this;
    }

    ~XValueStack() = default;

public:
    bool empty() const { return mStack_d.empty(); }

    T top() const { return mStack_d.top(); }

    T getXValue() const { return mXValue_d; }

    void push(const T &val) {
        if (mStack_d.empty() || mCmp_d(val, mXValue_d)) {
            mStack_d.push(mXValue_d); // save XValue
            mXValue_d = val;
        }
        mStack_d.push(val);
    }

    void pop() {
        T data = mStack_d.top(); mStack_d.pop();
        if (mXValue_d == data) {
            mXValue_d = mStack_d.top(); mStack_d.pop();
        }
        // return dstruct::move(data);
    }

    void clear() {
        mStack_d.clear();
    }

    // Iterator/range-for
    ConstIteratorType begin() const { return mStack_d.begin(); }
    ConstIteratorType end() const { return mStack_d.end(); }

protected:
    T mXValue_d;
    Compare mCmp_d;
    StackType mStack_d;
};

}
}

#endif