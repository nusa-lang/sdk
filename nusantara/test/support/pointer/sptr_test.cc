/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/support/pointer/sptr.h"
#include <gtest/gtest.h>
#include <stdexcept>
#include <utility>

TEST(SPTRTest, DefaultConstructor)
{
    nusantara::sptr<int> ptr;
    EXPECT_EQ(ptr == nullptr, true);
}

TEST(SPTRTest, ConstructorWithReference)
{
    int value = 42;
    nusantara::sptr<int> ptr(value);
    EXPECT_EQ(*ptr, 42);
}

TEST(SPTRTest, CopyConstructor)
{
    int value = 42;
    nusantara::sptr<int> ptr1(value);
    nusantara::sptr<int> ptr2 = ptr1;
    EXPECT_EQ(*ptr1, *ptr2);
}

TEST(SPTRTest, MoveConstructor)
{
    int value = 42;
    nusantara::sptr<int> ptr1(value);
    nusantara::sptr<int> ptr2 = std::move(ptr1);
    EXPECT_EQ(*ptr2, 42);
    EXPECT_EQ(ptr1 == nullptr, true);
}

TEST(SPTRTest, DereferenceOperator)
{
    int value = 42;
    nusantara::sptr<int> ptr(value);
    EXPECT_EQ(*ptr, 42);
}

TEST(SPTRTest, ArrowOperator)
{
    struct TestStruct
    {
        int value;
    };

    TestStruct testStruct{100};
    nusantara::sptr<TestStruct> ptr(testStruct);
    EXPECT_EQ(ptr->value, 100);
}

TEST(SPTRTest, NullptrDereference)
{
    nusantara::sptr<int> ptr;
    EXPECT_THROW(*ptr, std::runtime_error);
}

TEST(SPTRTest, MoveAssignmentOperator)
{
    int value = 42;
    nusantara::sptr<int> ptr1(value);
    nusantara::sptr<int> ptr2;
    ptr2 = std::move(ptr1);
    EXPECT_EQ(*ptr2, 42);
    EXPECT_EQ(ptr1 == nullptr, true);
}