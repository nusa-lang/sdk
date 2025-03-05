/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/lexer/lexer.h"
#include "nusantara/lexer/token/token.h"
#include "nusantara/lexer/token/token_type.h"
#include "nusantara/lexer/token/tokens.h"
#include <gtest/gtest.h>

TEST(LexerTest, InputConstructor)
{
    auto lexer{nusantara::Lexer::input("cetak('Hello World!')")};

    nusantara::Token token{lexer.nextToken()};
    token = lexer.nextToken();
    EXPECT_EQ(token.type, nusantara::TokenType::PAREN_OPEN);
}

TEST(LexerTest, FileConstructor)
{
    auto lexer{nusantara::Lexer::file("examples/helloworld/main.n")};

    nusantara::Token token{lexer.nextToken()};
    EXPECT_EQ(token.type, nusantara::TokenType::KW_IMPRT);
}

TEST(LexerTest, NextToken)
{
    auto lexer{nusantara::Lexer::file("examples/helloworld/main.n")};

    nusantara::Token token{lexer.nextToken()};
    EXPECT_EQ(token.type, nusantara::TokenType::KW_IMPRT);
    token = lexer.nextToken();
    EXPECT_EQ(token.type, nusantara::TokenType::IDENTIFIER);
    token = lexer.nextToken();
    EXPECT_EQ(token.type, nusantara::TokenType::IDENTIFIER);
}

TEST(LexerTest, GetTokens)
{
    auto lexer{nusantara::Lexer::file("examples/helloworld/main.n")};

    nusantara::Tokens tokens{lexer.getTokens()};

    EXPECT_EQ(tokens.source, "examples/helloworld/main.n");
    EXPECT_EQ(tokens.elements.size(), 7);
    EXPECT_EQ(tokens.elements[0].type, nusantara::TokenType::KW_IMPRT);
    EXPECT_EQ(tokens.elements[1].type, nusantara::TokenType::IDENTIFIER);
    EXPECT_EQ(tokens.elements[2].type, nusantara::TokenType::IDENTIFIER);
}

TEST(LexerTest, FileNotFound)
{
    EXPECT_ANY_THROW(nusantara::Lexer::file("examples/not_found.n"));
}