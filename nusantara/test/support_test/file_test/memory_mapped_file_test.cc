/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/support/file/memory_mapped_file.h"
#include <cstdio>
#include <fstream>
#include <gtest/gtest.h>
#include <string>
#include <utility>

class MemoryMappedFileTest : public ::testing::Test
{
protected:
    std::string testFilePath = "test_file.txt";

    void SetUp() override
    {
        std::ofstream outFile(testFilePath);
        outFile << "Hello, MemoryMappedFile!";
        outFile.close();
    }

    void TearDown() override
    {
        std::remove(testFilePath.c_str());
    }
};

TEST_F(MemoryMappedFileTest, CreateFileMapping)
{
    nusantara::MemoryMappedFile mmf = nusantara::MemoryMappedFile::create(testFilePath);
    EXPECT_NE(mmf.chars(), nullptr);
    EXPECT_EQ(mmf.size(), 24);
}

TEST_F(MemoryMappedFileTest, MoveConstructor)
{
    nusantara::MemoryMappedFile mmf1 = nusantara::MemoryMappedFile::create(testFilePath);
    nusantara::MemoryMappedFile mmf2(std::move(mmf1));

    EXPECT_NE(mmf2.chars(), nullptr);
    EXPECT_EQ(mmf2.size(), 24);
    EXPECT_EQ(mmf1.chars(), nullptr);
    EXPECT_EQ(mmf1.size(), 0);
}

TEST_F(MemoryMappedFileTest, MoveAssignment)
{
    nusantara::MemoryMappedFile mmf1 = nusantara::MemoryMappedFile::create(testFilePath);
    nusantara::MemoryMappedFile mmf2{std::move(mmf1)};

    EXPECT_NE(mmf2.chars(), nullptr);
    EXPECT_EQ(mmf2.size(), 24);
    EXPECT_EQ(mmf1.chars(), nullptr);
    EXPECT_EQ(mmf1.size(), 0);
}

TEST_F(MemoryMappedFileTest, EmptyFileMapping)
{
    std::ofstream emptyFile("empty_file.txt");
    emptyFile.close();

    nusantara::MemoryMappedFile mmf = nusantara::MemoryMappedFile::create("empty_file.txt");
    EXPECT_EQ(mmf.chars(), nullptr);
    EXPECT_EQ(mmf.size(), 0);

    std::remove("empty_file.txt");
}