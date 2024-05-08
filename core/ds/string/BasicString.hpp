// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef BASIC_STRING_HPP_DSTRUCT
#define BASIC_STRING_HPP_DSTRUCT

#include <core/common.hpp>
#include <core/ds/array/Vector.hpp>

namespace dstruct {

template <typename CharType, typename Alloc>
class BasicString {
private:
    using CharList_e = Vector<CharType, Alloc>;

    DSTRUCT_TYPE_SPEC_HELPER(CharList_e);

public:
    BasicString() : mCharList_d {1, '\0'} {
        mCharList_d.resize(15);
    }

    DSTRUCT_COPY_SEMANTICS(BasicString) {
        mCharList_d = ds.mCharList_d;
        return *this;
    }

    DSTRUCT_MOVE_SEMANTICS(BasicString) {
        mCharList_d = dstruct::move(ds.mCharList_d);
        return *this;
    }

    BasicString(const char *str) : BasicString() {
        mCharList_d[-1] = *str;
        while (*(++str) != '\0') {
            mCharList_d.push_back(*str);
        }
        mCharList_d.push_back('\0');
    }

    ~BasicString() = default;

    BasicString & operator=(const char *str) {
        *this = BasicString(str);
        return *this;
    }

public: // Capacity
    bool empty() const {
        return size() == 0;
    }

    SizeType size() const {
        return mCharList_d.size() - 1 /* '\0' */;
    }

    SizeType capacity() const {
        return mCharList_d.capacity();
    }

public: // Access
    ConstReferenceType back() const {
        return mCharList_d[-2];
    }

    ConstReferenceType front() const {
        return mCharList_d.front();
    }

    ConstReferenceType operator[](int index) const {
        return mCharList_d[index];
    }

public: // Modifiers
    void push_back(ConstReferenceType element) {
        mCharList_d[-1] = element;
        mCharList_d.push_back('\0');
    }

    ReferenceType operator[](int index) {
        return mCharList_d[index];
    }

    void clear() {
        mCharList_d.clear();
        mCharList_d.push_back('\0');
        mCharList_d.resize(15);
    }

public: // iterator/range-for support
    IteratorType begin() {
        return mCharList_d.begin();
    }

    ConstIteratorType begin() const {
        return mCharList_d.begin();
    }

    IteratorType end() {
        return mCharList_d.end() - 1;
    }

    ConstIteratorType end() const {
        return mCharList_d.end() - 1;
    }

public:
    ConstPointerType c_str() const {
        return &(mCharList_d[0]);
    }

    BasicString & operator+=(const BasicString &str) {
        if (mCharList_d.capacity() < mCharList_d.size() + str.mCharList_d.size() - 1)
            mCharList_d.resize(mCharList_d.size() + str.mCharList_d.size() - 1);
        mCharList_d[-1] = str[0];
        for (int i = 1; i < str.mCharList_d.size(); i++) {
            mCharList_d.push_back(str.mCharList_d[i]);
        }
        return *this;
    }

    BasicString & operator+=(const char * str) {
        return *this += BasicString(str);
    }

protected:
    CharList_e mCharList_d;
};

template <typename CharType, typename Alloc>
static BasicString<CharType, Alloc>
operator+(const BasicString<CharType, Alloc> s1, const BasicString<CharType, Alloc> s2) {
    BasicString<CharType, Alloc> s = s1;
    return s += s2;
}

template <typename CharType, typename Alloc>
static BasicString<CharType, Alloc>
operator+(const BasicString<CharType, Alloc> s1, const char *s2) {
    return operator+(s1, BasicString<CharType, Alloc>(s2));
}

template <typename CharType, typename Alloc>
static BasicString<CharType, Alloc>
operator+(const char *s1, const BasicString<CharType, Alloc> s2) {
    return operator+(BasicString<CharType, Alloc>(s1), s2);
}

template <typename CharType, typename Alloc>
static bool
operator==(const BasicString<CharType, Alloc> s1, const BasicString<CharType, Alloc> s2) {
    if (s1.size() != s2.size())
        return false;
    for (int i = 0; i < s1.size(); i++) {
        if (s1[i] != s2[i]) {
            return false;
        }
    }
    return true;
}

template <typename CharType, typename Alloc>
static bool
operator==(const BasicString<CharType, Alloc> s1, const char *s2) {
    return operator==(s1, BasicString<CharType, Alloc>(s2));
}

template <typename CharType, typename Alloc>
static bool
operator==(const char *s1, const BasicString<CharType, Alloc> s2) {
    return operator==(BasicString<CharType, Alloc>(s1), s2);
}

template <typename CharType, typename Alloc>
static bool
operator!=(const BasicString<CharType, Alloc> s1, const BasicString<CharType, Alloc> s2) {
    return !operator==(s1, s2);
}

template <typename CharType, typename Alloc>
static bool
operator!=(const BasicString<CharType, Alloc> s1, const char *s2) {
    return operator!=(s1, BasicString<CharType, Alloc>(s2));
}

template <typename CharType, typename Alloc>
static bool
operator!=(const char *s1, const BasicString<CharType, Alloc> s2) {
    return operator!=(BasicString<CharType, Alloc>(s1), s2);
}

}

#endif