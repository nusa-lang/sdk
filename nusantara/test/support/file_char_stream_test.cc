#include "nusantara/support/file_char_stream.h"
#include <cstdio>
#include <fstream>
#include <gtest/gtest.h>
#include <stdexcept>

const char* helloworldFilePath{"helloworld.n"};

void createHelloWorldFile(const bool& withNewLine = false)
{
    std::ofstream file{helloworldFilePath};

    if (!file.is_open())
        throw std::runtime_error("Failed to open file.");

    if (withNewLine)
        file << "Hello" << "\n" << "World!";
    else
        file << "Hello World!";

    file.close();
}

void removeHelloWorldFile()
{
    if (std::remove(helloworldFilePath) != 0)
        throw std::runtime_error("Failed to remove file.");
}

TEST(FileCharStreamTest, PeekFunctionTest)
{
    createHelloWorldFile();

    nusantara::FileCharStream fileCharStream{helloworldFilePath};

    EXPECT_EQ(*fileCharStream.peek(0), 'H');
    EXPECT_EQ(*fileCharStream.peek(1), 'e');
    EXPECT_EQ(*fileCharStream.peek(2), 'l');
    EXPECT_EQ(*fileCharStream.peek(3), 'l');
    EXPECT_EQ(*fileCharStream.peek(4), 'o');
    EXPECT_EQ(*fileCharStream.peek(5), ' ');
    EXPECT_EQ(*fileCharStream.peek(6), 'W');
    EXPECT_EQ(*fileCharStream.peek(7), 'o');
    EXPECT_EQ(*fileCharStream.peek(8), 'r');
    EXPECT_EQ(*fileCharStream.peek(9), 'l');
    EXPECT_EQ(*fileCharStream.peek(10), 'd');
    EXPECT_EQ(*fileCharStream.peek(11), '!');
    EXPECT_EQ(fileCharStream.peek(12), nullptr);

    removeHelloWorldFile();
}

TEST(FileCharStreamTest, NextFunctionTest)
{
    createHelloWorldFile();

    nusantara::FileCharStream fileCharStream{helloworldFilePath};

    EXPECT_EQ(*fileCharStream.next(), 'e');
    EXPECT_EQ(*fileCharStream.next(), 'l');
    EXPECT_EQ(*fileCharStream.next(), 'l');
    EXPECT_EQ(*fileCharStream.next(), 'o');
    EXPECT_EQ(*fileCharStream.next(), ' ');
    EXPECT_EQ(*fileCharStream.next(), 'W');
    EXPECT_EQ(*fileCharStream.next(), 'o');
    EXPECT_EQ(*fileCharStream.next(), 'r');
    EXPECT_EQ(*fileCharStream.next(), 'l');
    EXPECT_EQ(*fileCharStream.next(), 'd');
    EXPECT_EQ(*fileCharStream.next(), '!');
    EXPECT_EQ(fileCharStream.next(), nullptr);

    removeHelloWorldFile();
}

TEST(FileCharStreamTest, PreviousFunctionTest)
{
    createHelloWorldFile();

    nusantara::FileCharStream fileCharStream{helloworldFilePath};

    EXPECT_EQ(fileCharStream.previous(), nullptr);
    fileCharStream.next();
    EXPECT_EQ(*fileCharStream.previous(), 'H');

    removeHelloWorldFile();
}

TEST(FileCharStreamTest, CurrentFunctionTest)
{
    createHelloWorldFile();

    nusantara::FileCharStream fileCharStream{helloworldFilePath};

    EXPECT_EQ(*fileCharStream.current(), 'H');
    fileCharStream.next();
    EXPECT_EQ(*fileCharStream.current(), 'e');

    removeHelloWorldFile();
}

TEST(FileCharStreamTest, ResetFunctionTest)
{
    createHelloWorldFile();

    nusantara::FileCharStream fileCharStream{helloworldFilePath};

    EXPECT_EQ(*fileCharStream.current(), 'H');
    EXPECT_EQ(*fileCharStream.next(), 'e');
    EXPECT_EQ(*fileCharStream.next(), 'l');
    EXPECT_EQ(*fileCharStream.next(), 'l');
    EXPECT_EQ(*fileCharStream.next(), 'o');

    fileCharStream.reset();

    EXPECT_EQ(*fileCharStream.current(), 'H');
    EXPECT_EQ(*fileCharStream.next(), 'e');
    EXPECT_EQ(*fileCharStream.next(), 'l');
    EXPECT_EQ(*fileCharStream.next(), 'l');
    EXPECT_EQ(*fileCharStream.next(), 'o');

    removeHelloWorldFile();
}

TEST(FileCharStreamTest, ClearFunctionTest)
{
    createHelloWorldFile();

    nusantara::FileCharStream fileCharStream{helloworldFilePath};

    EXPECT_EQ(*fileCharStream.current(), 'H');
    EXPECT_EQ(*fileCharStream.next(), 'e');
    EXPECT_EQ(*fileCharStream.next(), 'l');
    EXPECT_EQ(*fileCharStream.next(), 'l');
    EXPECT_EQ(*fileCharStream.next(), 'o');

    fileCharStream.clear();

    EXPECT_EQ(fileCharStream.next(), nullptr);
    EXPECT_EQ(fileCharStream.next(), nullptr);

    removeHelloWorldFile();
}

TEST(FileCharStreamTest, IndexFunctionTest)
{
    createHelloWorldFile();

    nusantara::FileCharStream fileCharStream{helloworldFilePath};

    EXPECT_EQ(fileCharStream.index(), 0);

    fileCharStream.next();
    fileCharStream.next();

    EXPECT_EQ(fileCharStream.index(), 2);

    removeHelloWorldFile();
}

TEST(FileCharStreamTest, LineFunctionTest)
{
    createHelloWorldFile(true);

    nusantara::FileCharStream fileCharStream{helloworldFilePath};

    EXPECT_EQ(fileCharStream.line(), 0);

    fileCharStream.next(); // e
    fileCharStream.next(); // l
    fileCharStream.next(); // l
    fileCharStream.next(); // o
    fileCharStream.next(); // \n
    fileCharStream.next(); // W

    EXPECT_EQ(fileCharStream.line(), 1);

    removeHelloWorldFile();
}

TEST(FileCharStreamTest, ColumnFunctionTest)
{
    createHelloWorldFile();

    nusantara::FileCharStream fileCharStream{helloworldFilePath};

    EXPECT_EQ(fileCharStream.column(), 0);

    fileCharStream.next();
    fileCharStream.next();
    fileCharStream.next();
    fileCharStream.next();
    fileCharStream.next();

    EXPECT_EQ(fileCharStream.column(), 5);

    removeHelloWorldFile();
}