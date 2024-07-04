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

// Element Acess Test

TEST(Array, front_and_back) {
    BigFiveTest::clear_status();
    dstruct::Array<int, ARR_SIZE> arr1 = {2, 2, 3, 3};
    std::array<int, ARR_SIZE> arr2 = {2, 2, 3, 3};
    EXPECT_EQ(arr1.front(), arr2.front());
    EXPECT_EQ(arr1.back(), arr2.back());
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