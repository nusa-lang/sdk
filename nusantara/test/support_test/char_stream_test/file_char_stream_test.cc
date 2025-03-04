/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/support/char_stream/file_char_stream.h"
#include <cstdio>
#include <fstream>
#include <gtest/gtest.h>
#include <string>

class FileCharStreamTest : public ::testing::Test
{
protected:
    std::string testFilePath = "test_file.txt";
    std::string testEmptyFilePath = "test_empty_file.txt";

    void SetUp() override
    {
        std::ofstream outTestFile(testFilePath);
        outTestFile << "Hello\nWorld";
        outTestFile.close();

        std::ofstream outTestEmptyFile(testEmptyFilePath);
        outTestEmptyFile << "";
        outTestEmptyFile.close();
    }

    void TearDown() override
    {
        std::remove(testFilePath.c_str());
        std::remove(testEmptyFilePath.c_str());
    }
};

TEST_F(FileCharStreamTest, ConstructorWithFile)
{
    nusantara::FileCharStream stream(testFilePath);

    EXPECT_EQ(stream.index(), 0);
    EXPECT_EQ(stream.line(), 0);
    EXPECT_EQ(stream.column(), 0);
}

TEST_F(FileCharStreamTest, ConstructorWithEmptyFile)
{
    nusantara::FileCharStream stream(testEmptyFilePath);
    EXPECT_EQ(stream.current(), nullptr);
}

TEST_F(FileCharStreamTest, ConstructorWithNonExistentFile)
{
    EXPECT_ANY_THROW(nusantara::FileCharStream stream("nonexistent.txt"));
}

TEST_F(FileCharStreamTest, Peek)
{
    nusantara::FileCharStream stream(testFilePath);

    EXPECT_EQ(*stream.peek(0), 'H');
    EXPECT_EQ(*stream.peek(5), '\n');
    EXPECT_EQ(stream.peek(100), nullptr); // out of range
}

TEST_F(FileCharStreamTest, Current)
{
    nusantara::FileCharStream stream(testFilePath);

    EXPECT_EQ(*stream.current(), 'H');
    stream.next();
    EXPECT_EQ(*stream.current(), 'e');
}

TEST_F(FileCharStreamTest, Next)
{
    nusantara::FileCharStream stream(testFilePath);

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

TEST_F(FileCharStreamTest, Previous)
{
    nusantara::FileCharStream stream(testFilePath);

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

TEST_F(FileCharStreamTest, Reset)
{
    nusantara::FileCharStream stream(testFilePath);

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

TEST_F(FileCharStreamTest, Index)
{
    nusantara::FileCharStream stream(testFilePath);

    EXPECT_EQ(stream.index(), 0);
    stream.next(); // 'e'
    EXPECT_EQ(stream.index(), 1);
}

TEST_F(FileCharStreamTest, LineAndColumn)
{
    nusantara::FileCharStream stream(testFilePath);

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

TEST_F(FileCharStreamTest, GetPath)
{
    nusantara::FileCharStream stream(testFilePath);

    EXPECT_EQ(stream.path(), testFilePath);
}