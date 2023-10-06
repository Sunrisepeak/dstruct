// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef __DISJOINT_SET_BASE_HPP__DSTRUCT
#define __DISJOINT_SET_BASE_HPP__DSTRUCT

#include <core/common.hpp>
#include <core/ds/array/Vector.hpp>

namespace dstruct {

#define DEFAULT_TREE_HEIGHT -1

template <typename Alloc>
class DisjointSetBase {
private:
    using __Array = dstruct::Vector<int, Alloc>;

    DSTRUCT_TYPE_SPEC_HELPER(__Array)

public:
    DisjointSetBase(int n) : _mArray(n, DEFAULT_TREE_HEIGHT) { }
    DisjointSetBase(const DisjointSetBase &) = default;
    DisjointSetBase & operator=(const DisjointSetBase &) = default;
    DisjointSetBase(DisjointSetBase &&) = default;
    DisjointSetBase & operator=(DisjointSetBase &&) = default;
    ~DisjointSetBase() = default;

public: // Capacity
    bool empty() const {
        return _mArray.empty();
    }

    SizeType size() const {
        return _mArray.size();
    }

    SizeType capacity() const {
        return _mArray.capacity();
    }

public:
    void push() {
        _mArray.push_back(DEFAULT_TREE_HEIGHT); // Tree-Height is 1
    }

    void resize(int n) {
        _mArray.resize(n, DEFAULT_TREE_HEIGHT);
    }

public:
    ConstReferenceType find(ConstReferenceType index) {
        if (_mArray[index] < 0)
            return index;
        return _mArray[index] = find(_mArray[index]);
    }

    SizeType count() const {
        SizeType setsNum = 0;
        for (auto &v : _mArray) {
            if (v < 0)
                setsNum++;
        }
        return setsNum;
    }

public:
    bool connected(ConstReferenceType element1, ConstReferenceType element2) {
        return find(element1) == find(element2);
    }

    void connect(ConstReferenceType element1, ConstReferenceType element2) {
        ValueType root1 = find(element1);
        ValueType root2 = find(element2);
        union_set(root1, root2);
    }

    void union_set(ConstReferenceType root1, ConstReferenceType root2) {
        if (root1 < root2) {
            _mArray[root2] = root1;
        } else {
            if (_mArray[root1] == _mArray[root2])
                _mArray[root1]--;
            _mArray[root2] = root1;
        }
    }

protected:
    __Array _mArray;
};

}

#endif