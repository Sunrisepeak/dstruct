#ifndef ___XVALUE_STACK_HPP__DSTRUCT
#define ___XVALUE_STACK_HPP__DSTRUCT

#include <core/common.hpp>
#include <core/ds/stack/Stack.hpp>
#include <core/ds/array/Vector.hpp>

namespace dstruct {

namespace stack {

template<typename T, typename Compare, typename StackType = adapter::Stack<T, Vector<T>>>
class XValueStack {

    DSTRUCT_TYPE_SPEC_DECLARE(StackType)

public:
    XValueStack() = default;
    XValueStack(const Compare &cmp) : _mCmp { cmp } { }

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

    T pop() {
        T data = _mStack.pop();
        if (_mXValue == data) {
            _mXValue = _mStack.pop();
        }
        return dstruct::move(data);
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