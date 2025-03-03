#include "nusantara/support/char_stream.h"
#include <gtest/gtest.h>

TEST(CharStreamTest, PeekFunctionTest)
{
    nusantara::CharStream charStream{"Hello World!"};

    EXPECT_EQ(*charStream.peek(0), 'H');
    EXPECT_EQ(*charStream.peek(1), 'e');
    EXPECT_EQ(*charStream.peek(2), 'l');
    EXPECT_EQ(*charStream.peek(3), 'l');
    EXPECT_EQ(*charStream.peek(4), 'o');
    EXPECT_EQ(*charStream.peek(5), ' ');
    EXPECT_EQ(*charStream.peek(6), 'W');
    EXPECT_EQ(*charStream.peek(7), 'o');
    EXPECT_EQ(*charStream.peek(8), 'r');
    EXPECT_EQ(*charStream.peek(9), 'l');
    EXPECT_EQ(*charStream.peek(10), 'd');
    EXPECT_EQ(*charStream.peek(11), '!');
    EXPECT_EQ(charStream.peek(12), nullptr);
}

TEST(CharStreamTest, NextFunctionTest)
{
    nusantara::CharStream charStream{"Hello World!"};

    EXPECT_EQ(*charStream.next(), 'e');
    EXPECT_EQ(*charStream.next(), 'l');
    EXPECT_EQ(*charStream.next(), 'l');
    EXPECT_EQ(*charStream.next(), 'o');
    EXPECT_EQ(*charStream.next(), ' ');
    EXPECT_EQ(*charStream.next(), 'W');
    EXPECT_EQ(*charStream.next(), 'o');
    EXPECT_EQ(*charStream.next(), 'r');
    EXPECT_EQ(*charStream.next(), 'l');
    EXPECT_EQ(*charStream.next(), 'd');
    EXPECT_EQ(*charStream.next(), '!');
    EXPECT_EQ(charStream.next(), nullptr);
}

TEST(CharStreamTest, PreviousFunctionTest)
{
    nusantara::CharStream charStream{"Hello World!"};

    EXPECT_EQ(charStream.previous(), nullptr);
    charStream.next();
    EXPECT_EQ(*charStream.previous(), 'H');
}

TEST(CharStreamTest, CurrentFunctionTest)
{
    nusantara::CharStream charStream{"Hello World!"};

    EXPECT_EQ(*charStream.current(), 'H');
    charStream.next();
    EXPECT_EQ(*charStream.current(), 'e');
}

TEST(CharStreamTest, ResetFunctionTest)
{
    nusantara::CharStream charStream{"Hello World!"};

    EXPECT_EQ(*charStream.current(), 'H');
    EXPECT_EQ(*charStream.next(), 'e');
    EXPECT_EQ(*charStream.next(), 'l');
    EXPECT_EQ(*charStream.next(), 'l');
    EXPECT_EQ(*charStream.next(), 'o');

    charStream.reset();

    EXPECT_EQ(*charStream.current(), 'H');
    EXPECT_EQ(*charStream.next(), 'e');
    EXPECT_EQ(*charStream.next(), 'l');
    EXPECT_EQ(*charStream.next(), 'l');
    EXPECT_EQ(*charStream.next(), 'o');
}

TEST(CharStreamTest, ClearFunctionTest)
{
    nusantara::CharStream charStream{"Hello World!"};

    EXPECT_EQ(*charStream.current(), 'H');
    EXPECT_EQ(*charStream.next(), 'e');
    EXPECT_EQ(*charStream.next(), 'l');
    EXPECT_EQ(*charStream.next(), 'l');
    EXPECT_EQ(*charStream.next(), 'o');

    charStream.clear();

    EXPECT_EQ(charStream.next(), nullptr);
    EXPECT_EQ(charStream.next(), nullptr);
}

TEST(CharStreamTest, IndexFunctionTest)
{
    nusantara::CharStream charStream{"Hello World!"};

    EXPECT_EQ(charStream.index(), 0);

    charStream.next();
    charStream.next();

    EXPECT_EQ(charStream.index(), 2);
}

TEST(CharStreamTest, LineFunctionTest)
{
    nusantara::CharStream charStream{"Hello\nWorld!"};

    EXPECT_EQ(charStream.line(), 0);

    charStream.next(); // e
    charStream.next(); // l
    charStream.next(); // l
    charStream.next(); // o
    charStream.next(); // \n
    charStream.next(); // W

    EXPECT_EQ(charStream.line(), 1);
}

TEST(CharStreamTest, ColumnFunctionTest)
{
    nusantara::CharStream charStream{"Hello World!"};

    EXPECT_EQ(charStream.column(), 0);

    charStream.next();
    charStream.next();
    charStream.next();
    charStream.next();
    charStream.next();

    EXPECT_EQ(charStream.column(), 5);
}