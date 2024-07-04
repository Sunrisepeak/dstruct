// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/DStruct
//

#include <gtest/gtest.h>

#include <dstruct.hpp>

#include "TestBase.hpp"

using dstruct::BigFiveTest;

constexpr int ARR_SIZE = 10;

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