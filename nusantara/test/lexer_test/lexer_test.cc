/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/lexer/lexer.h"
#include "nusantara/lexer/token/token_type.h"
#include "nusantara/lexer/token/tokens.h"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <string>
#include <vector>

class LexerTest : public ::testing::Test
{
protected:
    std::string input0{"muat 'cetak.n'\ncetak('Hello World!')"};
    std::string input1{"luar f cetak(teks nilai)"};
    std::string file0{"main.n"};
    std::string file1{"cetak.n"};
    std::string fileNotFound{"not_found.n"};

    void SetUp() override
    {
        std::ofstream outFile0(file0);
        outFile0 << input0;
        outFile0.close();

        std::ofstream outFile1(file1);
        outFile1 << input1;
        outFile1.close();
    }

    void TearDown() override
    {
        std::remove(file0.c_str());
        std::remove(file1.c_str());
    }
};

TEST_F(LexerTest, Input)
{
    auto lexer{nusantara::Lexer::input(this->input1)};

    std::vector<nusantara::Tokens> tokens{lexer.loadTokens()};

    EXPECT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].elements[1].type, nusantara::TokenType::KW_FUNC);
}

TEST_F(LexerTest, File)
{
    auto lexer{nusantara::Lexer::file(this->file1)};

    std::vector<nusantara::Tokens> tokens{lexer.loadTokens()};
    EXPECT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].source, std::filesystem::weakly_canonical(this->file1));
    EXPECT_EQ(tokens[0].elements[0].type, nusantara::TokenType::KW_EXTERN);
}

TEST_F(LexerTest, LoadTokens)
{
    auto lexer{nusantara::Lexer::file(this->file0)};

    std::vector<nusantara::Tokens> tokens{lexer.loadTokens()};

    EXPECT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].source, std::filesystem::weakly_canonical(this->file1));
    EXPECT_EQ(tokens[1].source, std::filesystem::weakly_canonical(this->file0));
    EXPECT_EQ(tokens[0].elements.size(), 8);
    EXPECT_EQ(tokens[1].elements.size(), 7);
    EXPECT_EQ(tokens[0].elements[0].type, nusantara::TokenType::KW_EXTERN);
    EXPECT_EQ(tokens[1].elements[1].type, nusantara::TokenType::LIT_STR);
    EXPECT_EQ(tokens[0].elements[2].type, nusantara::TokenType::IDENTIFIER);
}

TEST_F(LexerTest, FileNotFound)
{
    EXPECT_ANY_THROW(nusantara::Lexer::file(this->fileNotFound));
}