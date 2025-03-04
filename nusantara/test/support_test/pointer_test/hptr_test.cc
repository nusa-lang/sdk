/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/support/pointer/hptr.h"
#include <gtest/gtest.h>
#include <stdexcept>
#include <utility>

TEST(HPTRTest, DefaultConstructor)
{
    nusantara::hptr<int> ptr;
    EXPECT_EQ(ptr == nullptr, true);
}

TEST(HPTRTest, ConstructorWithValue)
{
    int value = 42;
    nusantara::hptr<int> ptr(value);
    EXPECT_EQ(*ptr, 42);
}

TEST(HPTRTest, CopyConstructor)
{
    int value = 42;
    nusantara::hptr<int> ptr1(value);
    nusantara::hptr<int> ptr2 = ptr1;
    EXPECT_EQ(*ptr1, *ptr2);
}

TEST(HPTRTest, MoveConstructor)
{
    int value = 42;
    nusantara::hptr<int> ptr1(value);
    nusantara::hptr<int> ptr2 = std::move(ptr1);
    EXPECT_EQ(*ptr2, 42);
    EXPECT_EQ(ptr1 == nullptr, true);
}

TEST(HPTRTest, DereferenceOperator)
{
    int value = 42;
    nusantara::hptr<int> ptr(value);
    EXPECT_EQ(*ptr, 42);
}

TEST(HPTRTest, ArrowOperator)
{
    struct TestStruct
    {
        int value;
    };

    TestStruct testStruct{100};
    nusantara::hptr<TestStruct> ptr(testStruct);
    EXPECT_EQ(ptr->value, 100);
}

TEST(HPTRTest, NullptrDereference)
{
    nusantara::hptr<int> ptr;
    EXPECT_THROW(*ptr, std::runtime_error);
}

TEST(HPTRTest, DelMethod)
{
    nusantara::hptr<int> ptr(42);
    EXPECT_NO_THROW(ptr.del());
    EXPECT_EQ(ptr == nullptr, true);
}

TEST(HPTRTest, MoveAssignmentOperator)
{
    int value = 42;
    nusantara::hptr<int> ptr1(value);
    nusantara::hptr<int> ptr2;
    ptr2 = std::move(ptr1);
    EXPECT_EQ(*ptr2, 42);
    EXPECT_EQ(ptr1 == nullptr, true);
}