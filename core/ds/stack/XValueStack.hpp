// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef ___XVALUE_STACK_HPP__DSTRUCT
#define ___XVALUE_STACK_HPP__DSTRUCT

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
    XValueStack(const Compare &cmp) : _mCmp { cmp } { }

    DSTRUCT_COPY_SEMANTICS(XValueStack) {
        this->_mXValue = ds._mXValue;
        this->_mCmp = ds._mCmp;
        this->_mStack = ds._mStack;
        return *this;
    }

    DSTRUCT_MOVE_SEMANTICS(XValueStack) {
        _mXValue = dstruct::move(ds._mXValue);
        _mCmp =  dstruct::move(ds._mCmp);
        _mStack =  dstruct::move(ds._mStack);
        return *this;
    }

    ~XValueStack() = default;

public:
    bool empty() const { return _mStack.empty(); }

    T top() const { return _mStack.top(); }

    T getXValue() const { return _mXValue; }

    void push(const T &val) {
        if (_mStack.empty() || _mCmp(val, _mXValue)) {
            _mStack.push(_mXValue); // save XValue
            _mXValue = val;
        }
        _mStack.push(val);
    }

    void pop() {
        T data = _mStack.top(); _mStack.pop();
        if (_mXValue == data) {
            _mXValue = _mStack.top(); _mStack.pop();
        }
        // return dstruct::move(data);
    }

    void clear() {
        _mStack.clear();
    }

    // Iterator/range-for
    ConstIteratorType begin() const { return _mStack.begin(); }
    ConstIteratorType end() const { return _mStack.end(); }

protected:
    T _mXValue;
    Compare _mCmp;
    StackType _mStack;
};

}
}

#endif