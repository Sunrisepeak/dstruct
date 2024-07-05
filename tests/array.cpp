// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#include <array>

#include <gtest/gtest.h>

#include <dstruct.hpp>

#include "TestBase.hpp"

using dstruct::BigFiveTest;

constexpr int ARR_SIZE = 10;

// Base Test

TEST(Array, destructor) {
    BigFiveTest::clear_status();
    {
        dstruct::Array<BigFiveTest::Obj, ARR_SIZE> arr;
        EXPECT_EQ(BigFiveTest::constructor(), ARR_SIZE);
    }
    EXPECT_EQ(BigFiveTest::destructor(), ARR_SIZE);
}

TEST(Array, copy_sema) {
    BigFiveTest::clear_status();
    {
        dstruct::Array<BigFiveTest::Obj, ARR_SIZE> arr1;
        EXPECT_EQ(BigFiveTest::constructor(), ARR_SIZE);
        dstruct::Array<BigFiveTest::Obj, ARR_SIZE> arr2(arr1);
        EXPECT_EQ(BigFiveTest::copy_constructor(), ARR_SIZE);
        arr1 = arr2;
        EXPECT_EQ(BigFiveTest::copy_assignment(), ARR_SIZE);
        EXPECT_EQ(BigFiveTest::destructor(), ARR_SIZE);
    }
    EXPECT_EQ(BigFiveTest::destructor(), 3 * ARR_SIZE);
}

TEST(Array, move_sema) {
    BigFiveTest::clear_status();
    {
        dstruct::Array<BigFiveTest::Obj, ARR_SIZE> arr1;
        EXPECT_EQ(BigFiveTest::constructor(), ARR_SIZE);
        dstruct::Array<BigFiveTest::Obj, ARR_SIZE> arr2(std::move(arr1));
        EXPECT_EQ(BigFiveTest::constructor(), ARR_SIZE);
        EXPECT_EQ(BigFiveTest::move_constructor(), ARR_SIZE);
        EXPECT_EQ(BigFiveTest::destructor(), 0);
        arr1 = std::move(arr2);
        EXPECT_EQ(BigFiveTest::move_assignment(), ARR_SIZE);
        EXPECT_EQ(BigFiveTest::destructor(), 0);
    }
    EXPECT_EQ(BigFiveTest::destructor(), ARR_SIZE);
}

TEST(Array, initialization_list) {
    BigFiveTest::clear_status();
    {
        dstruct::Array<int, ARR_SIZE> arr1 = {2, 2, 3, 3};
        std::array<int, ARR_SIZE> arr2 = {2, 2, 3, 3};
        for (int i = 0; i < ARR_SIZE; i++) {
            EXPECT_EQ(arr1[i], arr2[i]);
        }
    }
    {
        dstruct::Array<double, 5> arr1 = {1, 2, 3, 4, 5, 6 /*unused*/};
        std::array<double, 5> arr2 = {1, 2, 3, 4, 5};
        for (int i = 0; i < 5; i++) {
            EXPECT_EQ(arr1[i], arr2[i]);
        }
    }
}

// Capacity Test

TEST(Array, capacity) {
    {
        dstruct::Array<int, ARR_SIZE> arr1;
        std::array<int, ARR_SIZE> arr2;
        EXPECT_EQ(arr1.empty(), arr2.empty());
        EXPECT_EQ(arr1.size(), arr2.size());
    }
    {
        dstruct::Array<int, 0> arr1;
        std::array<int, 0> arr2;
        EXPECT_EQ(arr1.empty(), arr2.empty());
        EXPECT_EQ(arr1.size(), arr2.size());
        EXPECT_EQ(arr1.capacity(), 1); // special for dstruct::Array
    }
}

// Element Acess Test

TEST(Array, front_and_back) {
    {
        dstruct::Array<int, ARR_SIZE> arr1 = {2, 2, 3, 3};
        std::array<int, ARR_SIZE> arr2 = {2, 2, 3, 3};
        EXPECT_EQ(arr1.front(), arr2.front());
        EXPECT_EQ(arr1.back(), arr2.back());
        dstruct::swap(arr1.front(), arr1.back());
        EXPECT_EQ(arr1.back(), arr2.front());
        EXPECT_EQ(arr2.back(), arr1.front());
    }

    {
        const dstruct::Array<int, ARR_SIZE> arr = {2, 2, 3, 3};
        static_assert(dstruct::IsConst<decltype(arr.front())>::value);
        static_assert(dstruct::IsConst<decltype(arr.back())>::value);
    }
}

TEST(Array, subscript_operator) {
    {
        dstruct::Array<int, ARR_SIZE> arr1 = {2, 2, 3, 3};
        std::array<int, ARR_SIZE> arr2 = {2, 2, 3, 3};
        for (int i = 0, j = -ARR_SIZE; i < arr1.size(); i++, j++) {
            EXPECT_EQ(arr1[i], arr2[i]);
            EXPECT_EQ(arr1[j], arr2[i]);
        }
        arr1[-1] = arr2.back() = 1;
        EXPECT_EQ(arr1.back(), arr2.back());
    }

    {
        const dstruct::Array<int, ARR_SIZE> arr = {2, 2, 3, 3};
        static_assert(dstruct::IsConst<decltype(arr[0])>::value);
    }
}

TEST(Array, data) {
    {
        dstruct::Array<int, ARR_SIZE> arr = {2, 2, 3, 3};
        auto intPtr = arr.data();
        for (int i = 0; i < arr.size(); i++) {
            EXPECT_EQ(intPtr[i], arr[i]);
        }

        arr.data()[2] = 4;
        EXPECT_EQ(4, arr[2]);
    }

    {
        const dstruct::Array<int, ARR_SIZE> arr = {2, 2, 3, 3};
        static_assert(dstruct::IsConst<decltype(arr.data())>::value);
    }
}

// Iterator

TEST(Array, iterator) {
    {
        dstruct::Array<int, 4> arr1 = {2, 2, 3, 3};
        dstruct::Array<int, 4> arr2 = {2, 2, 3, 3};

        auto it = arr1.begin();
        for (int i = 0; i < arr1.size(); i++, it++) {
            EXPECT_EQ(*it, arr1[i]);
            *it *= 2;
        }
        EXPECT_EQ(it, arr1.end());

        int index = 0;
        for (auto const_it = arr1.cbegin(); const_it != arr1.cend(); ++const_it, ++index) {
            EXPECT_EQ(*const_it, arr2[index] * 2);
        }
    }

    {
        const dstruct::Array<int, ARR_SIZE> arr = {2, 2, 3, 3};
        static_assert(dstruct::IsConst<decltype(*(arr.begin()))>::value);
    }
}

// Method chaining

TEST(Array, sort) {
    BigFiveTest::clear_status();
    dstruct::Array<int, 6> arr = {3, 2, 1, 5, 4, 0};
    arr.sort();
    for (int i = 0; i < 6; i++) {
        EXPECT_EQ(arr[i], i);
    }

    int cmpVal = 5;
    for (auto val : arr.sort([](auto a, auto b) { return b < a; })) {
        EXPECT_EQ(val, cmpVal);
        cmpVal--;
    }
}

TEST(Array, reverse) {
    dstruct::Array<int, 6> arr = {3, 2, 1, 5, 4, 0};
    arr.sort().reverse();
    for (int i = 0; i < arr.size(); i++) {
        EXPECT_EQ(arr[i], arr.size() - 1 - i);
    }
}

TEST(Array, swap) {
    dstruct::Array<int, 6> arr1 = {3, 2, 1, 5, 4, 0};
    auto arr2 = arr1;
    int oldBackValue = arr1.back();
    arr1.sort();
    for (int i = 0; i < arr1.size(); i++) {
        EXPECT_EQ(arr1[i], i);
    }
    arr2.swap(arr1);
    for (int i = 0; i < arr2.size(); i++) {
        EXPECT_EQ(arr2[i], i);
    }
    EXPECT_EQ(arr1[-1], oldBackValue);
}