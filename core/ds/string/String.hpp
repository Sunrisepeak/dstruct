// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#ifndef __STRING_HPP__DSTRUCT
#define __STRING_HPP__DSTRUCT

#include <core/common.hpp>
#include <core/ds/array/Vector.hpp>

namespace dstruct {

// simple implement for string
class String {
private:
    using __CharList = Vector<char>;

    DSTRUCT_TYPE_SPEC_HELPER(__CharList);

public:
    String() : _mCharList {1, '\0'} { _mCharList.resize(15); }

    DSTRUCT_COPY_SEMANTICS(String) {
        _mCharList = ds._mCharList;
        return *this;
    }

    DSTRUCT_MOVE_SEMANTICS(String) {
        _mCharList = dstruct::move(ds._mCharList);
        return *this;
    }

    String(const char *str) : String() {
        _mCharList[-1] = *str;
        while (*(++str) != '\0') {
            _mCharList.push_back(*str);
        }
        _mCharList.push_back('\0');
    }

    String & operator=(const char *str) {
        *this = String(str);
        return *this;
    }

public: // Capacity
    bool empty() const {
        return size() == 0;
    }

    SizeType size() const {
        return _mCharList.size() - 1 /* '\0' */;
    }

    SizeType capacity() const {
        return _mCharList.capacity();
    }

public: // Access
    ConstReferenceType back() const {
        return _mCharList[-2];
    }

    ConstReferenceType front() const {
        return _mCharList.front();
    }

    ConstReferenceType operator[](int index) const {
        return _mCharList[index];
    }

public: // Modifiers
    void push_back(ConstReferenceType element) {
        _mCharList[-1] = element;
        _mCharList.push_back('\0');
    }

    ReferenceType operator[](int index) {
        return _mCharList[index];
    }

public: // iterator/range-for support
    IteratorType begin() {
        return _mCharList.begin();
    }

    ConstIteratorType begin() const {
        return _mCharList.begin();
    }

    IteratorType end() {
        return _mCharList.end() - 1;
    }

    ConstIteratorType end() const {
        return _mCharList.end() - 1;
    }

public:
    ConstPointerType c_str() const {
        return &(_mCharList[0]);
    }

    String & operator+=(const String &str) {
        if (_mCharList.capacity() < _mCharList.size() + str._mCharList.size() - 1)
            _mCharList.resize(_mCharList.size() + str._mCharList.size() - 1);
        _mCharList[-1] = str[0];
        for (int i = 1; i < str._mCharList.size(); i++) {
            _mCharList.push_back(str._mCharList[i]);
        }
        return *this;
    }

    String & operator+=(const char * str) {
        return *this += String(str);
    }

protected:
    __CharList _mCharList;
};

String operator+(const String s1, const String s2) {
    String s = s1;
    return s += s2;
}

String operator+(const String s1, const char *s2) {
    return operator+(s1, String(s2));
}

String operator+(const char *s1, const String s2) {
    return operator+(String(s1), s2);
}

bool operator==(const String s1, const String s2) {
    if (s1.size() != s2.size())
        return false;
    for (int i = 0; i < s1.size(); i++) {
        if (s1[i] != s2[i]) {
            return false;
        }
    }
    return true;
}

bool operator==(const String s1, const char *s2) {
    return operator==(s1, String(s2));
}

bool operator==(const char *s1, const String s2) {
    return operator==(String(s1), s2);
}

bool operator!=(const String s1, const String s2) {
    return !operator==(s1, s2);
}

bool operator!=(const String s1, const char *s2) {
    return operator!=(s1, String(s2));
}

bool operator!=(const char *s1, const String s2) {
    return operator!=(String(s1), s2);
}

}

#endif