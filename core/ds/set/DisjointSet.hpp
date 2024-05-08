// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef DISJOINT_SET_HPP__DSTRUCT
#define DISJOINT_SET_HPP__DSTRUCT

#include <core/common.hpp>
#include <core/ds/array/Array.hpp>

namespace dstruct {

// only for key(int: 0 ~ N)
template <typename Alloc>
class DisjointSet {
private:
    using __Array = dstruct::Vector<int, Alloc>;

    DSTRUCT_TYPE_SPEC_HELPER(__Array)

public:
    DisjointSet(int n) : mArray_d(n, -1) { }
    DisjointSet(const DisjointSet &) = default;
    DisjointSet & operator=(const DisjointSet &) = default;
    DisjointSet(DisjointSet &&) = default;
    DisjointSet & operator=(DisjointSet &&) = default;
    ~DisjointSet() = default;

public: // Capacity
    bool empty() const {
        return mArray_d.empty();
    }

    SizeType size() const {
        return mArray_d.size();
    }

    SizeType capacity() const {
        return mArray_d.capacity();
    }

public:
    ConstReferenceType find(ConstReferenceType index) {
        if (mArray_d[index] < 0)
            return index;
        return mArray_d[index] = find(mArray_d[index]);
    }

    SizeType count() const {
        SizeType setsNum = 0;
        for (auto &v : mArray_d) {
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
            mArray_d[root2] = root1;
        } else {
            if (mArray_d[root1] == mArray_d[root2])
                mArray_d[root1]--;
            mArray_d[root2] = root1;
        }
    }

protected:
    __Array mArray_d;
};

}

#endif