/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/support/file_char_stream.h"
#include <cstdio>
#include <gtest/gtest.h>
#include <llvm/Support/raw_ostream.h>
#include <stdexcept>
#include <string>
#include <system_error>
#include <utility>

class TestFile
{
public:
    explicit TestFile(std::string filePath, std::string content) : _filePath(std::move(filePath)), _content(std::move(content)) {}

    void create()
    {
        std::error_code errCode;
        llvm::raw_fd_ostream fileStream{this->_filePath, errCode};

        fileStream << this->_content;

        if (errCode)
            throw std::runtime_error("Gagal membuat file.");
    }

    void remove()
    {
        if (std::remove(this->_filePath.c_str()) != 0)
            throw std::runtime_error("Gagal menghapus file.");
    }

    [[nodiscard]] const std::string& filePath() const
    {
        return this->_filePath;
    }

private:
    std::string _filePath;
    std::string _content;
};

TEST(FileCharStreamTest, ConstructorWithFile)
{
    TestFile testFile{"test.txt", "Hello\nFileCharStream\n"};

    EXPECT_NO_THROW(testFile.create());

    nusantara::FileCharStream stream(testFile.filePath());
    EXPECT_EQ(*stream.current(), 'H');
    EXPECT_EQ(*stream.next(), 'e');
    EXPECT_EQ(*stream.next(), 'l');
    EXPECT_EQ(*stream.next(), 'l');
    EXPECT_EQ(*stream.next(), 'o');
    EXPECT_EQ(*stream.next(), '\n');
    EXPECT_EQ(*stream.next(), 'F');

    EXPECT_EQ(stream.line(), 1);
    EXPECT_EQ(stream.column(), 0);

    EXPECT_EQ(*stream.next(), 'i');
    EXPECT_EQ(*stream.next(), 'l');
    EXPECT_EQ(*stream.next(), 'e');
    EXPECT_EQ(*stream.next(), 'C');
    EXPECT_EQ(*stream.next(), 'h');
    EXPECT_EQ(*stream.next(), 'a');
    EXPECT_EQ(*stream.next(), 'r');
    EXPECT_EQ(*stream.next(), 'S');
    EXPECT_EQ(*stream.next(), 't');
    EXPECT_EQ(*stream.next(), 'r');
    EXPECT_EQ(*stream.next(), 'e');
    EXPECT_EQ(*stream.next(), 'a');
    EXPECT_EQ(*stream.next(), 'm');
    EXPECT_EQ(*stream.next(), '\n');

    EXPECT_NO_THROW(testFile.remove());
}

TEST(FileCharStreamTest, ConstructorWithEmptyFile)
{
    TestFile testFile{"empty.txt", ""};

    EXPECT_NO_THROW(testFile.create());

    nusantara::FileCharStream stream(testFile.filePath());
    EXPECT_EQ(stream.current(), nullptr);

    EXPECT_NO_THROW(testFile.remove());
}

TEST(FileCharStreamTest, ConstructorWithNonExistentFile)
{
    EXPECT_ANY_THROW(nusantara::FileCharStream stream("nonexistent.txt"));
}
