/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/support/char_stream.h"
#include <gtest/gtest.h>
#include <string>

TEST(CharStreamTest, DefaultConstructor)
{
    nusantara::CharStream stream;
    EXPECT_EQ(stream.index(), 0);
    EXPECT_EQ(stream.line(), 0);
    EXPECT_EQ(stream.column(), 0);
}

TEST(CharStreamTest, ConstructorWithString)
{
    std::string chars = "Hello\nWorld";
    nusantara::CharStream stream(chars);

    EXPECT_EQ(stream.index(), 0);
    EXPECT_EQ(stream.line(), 0);
    EXPECT_EQ(stream.column(), 0);
}

TEST(CharStreamTest, Peek)
{
    std::string chars = "Hello\nWorld";
    nusantara::CharStream stream(chars);

    EXPECT_EQ(*stream.peek(0), 'H');
    EXPECT_EQ(*stream.peek(5), '\n');
    EXPECT_EQ(stream.peek(100), nullptr); // out of range
}

TEST(CharStreamTest, Current)
{
    std::string chars = "Hello\nWorld";
    nusantara::CharStream stream(chars);

    EXPECT_EQ(*stream.current(), 'H');
    stream.next();
    EXPECT_EQ(*stream.current(), 'e');
}

TEST(CharStreamTest, Next)
{
    std::string chars = "Hello\nWorld";
    nusantara::CharStream stream(chars);

    EXPECT_EQ(*stream.next(), 'e');
    EXPECT_EQ(stream.index(), 1);
    EXPECT_EQ(stream.line(), 0);
    EXPECT_EQ(stream.column(), 1);

    stream.next(); // 'l'
    stream.next(); // 'l'
    stream.next(); // 'o'
    stream.next(); // '\n'
    stream.next(); // 'W'
    EXPECT_EQ(stream.line(), 1);
    EXPECT_EQ(stream.column(), 0);

    EXPECT_EQ(*stream.next(), 'o');
    EXPECT_EQ(stream.index(), 7);
}

TEST(CharStreamTest, Previous)
{
    std::string chars = "Hello\nWorld";
    nusantara::CharStream stream(chars);

    stream.next(); // 'e'
    stream.next(); // 'l'
    stream.next(); // 'l'
    stream.next(); // 'o'

    EXPECT_EQ(*stream.previous(), 'l');
    EXPECT_EQ(stream.index(), 3);
    EXPECT_EQ(stream.line(), 0);
    EXPECT_EQ(stream.column(), 3);

    EXPECT_EQ(*stream.previous(), 'l');
    EXPECT_EQ(stream.index(), 2);
    EXPECT_EQ(stream.line(), 0);
    EXPECT_EQ(stream.column(), 2);

    EXPECT_EQ(*stream.previous(), 'e');
    EXPECT_EQ(stream.index(), 1);
    EXPECT_EQ(stream.line(), 0);
    EXPECT_EQ(stream.column(), 1);

    EXPECT_EQ(*stream.previous(), 'H');
    EXPECT_EQ(stream.index(), 0);
    EXPECT_EQ(stream.line(), 0);
    EXPECT_EQ(stream.column(), 0);

    EXPECT_EQ(stream.previous(), nullptr); // no previous character
}

TEST(CharStreamTest, Reset)
{
    std::string chars = "Hello\nWorld";
    nusantara::CharStream stream(chars);

    stream.next(); // 'e'
    stream.next(); // 'l'
    stream.next(); // 'l'

    EXPECT_EQ(stream.index(), 3);
    EXPECT_EQ(stream.line(), 0);
    EXPECT_EQ(stream.column(), 3);

    stream.reset();

    EXPECT_EQ(stream.index(), 0);
    EXPECT_EQ(stream.line(), 0);
    EXPECT_EQ(stream.column(), 0);
}

TEST(CharStreamTest, Clear)
{
    std::string chars = "Hello\nWorld";
    nusantara::CharStream stream(chars);

    stream.clear();

    EXPECT_EQ(stream.index(), 0);
    EXPECT_EQ(stream.line(), 0);
    EXPECT_EQ(stream.column(), 0);
    EXPECT_EQ(stream.peek(0), nullptr); // characters are cleared
}

TEST(CharStreamTest, Index)
{
    std::string chars = "Hello\nWorld";
    nusantara::CharStream stream(chars);

    EXPECT_EQ(stream.index(), 0);
    stream.next(); // 'e'
    EXPECT_EQ(stream.index(), 1);
}

TEST(CharStreamTest, LineAndColumn)
{
    std::string chars = "Hello\nWorld";
    nusantara::CharStream stream(chars);

    EXPECT_EQ(stream.line(), 0);
    EXPECT_EQ(stream.column(), 0);

    stream.next(); // 'e'
    EXPECT_EQ(stream.line(), 0);
    EXPECT_EQ(stream.column(), 1);

    stream.next(); // 'l'
    stream.next(); // 'l'
    stream.next(); // 'o'
    stream.next(); // '\n'
    stream.next(); // 'W'

    EXPECT_EQ(stream.line(), 1);
    EXPECT_EQ(stream.column(), 0);

    stream.next(); // 'o'
    EXPECT_EQ(stream.line(), 1);
    EXPECT_EQ(stream.column(), 1);
}