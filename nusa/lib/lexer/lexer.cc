/*
 * ----------------------------------------------------------------------------
 * Project: Nusa
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, The Nusa Project Authors
 * ----------------------------------------------------------------------------
 */

#include "nusa/lexer/lexer.h"
#include "nusa/lexer/token/token.h"
#include "nusa/lexer/token/token_type.h"
#include "nusa/lexer/token/tokens.h"
#include "nusa/lexer/use_manager.h"
#include "nusa/support/char_stream.h"
#include "nusa/support/diagnostic/diagnostic_category.h"
#include "nusa/support/diagnostic/diagnostic_module.h"
#include "nusa/support/diagnostic/diagnostics.h"
#include <array>
#include <cctype>
#include <cstddef>
#include <cstring>
#include <exception>
#include <string>
#include <utility>
#include <vector>

namespace nusa {

Lexer::Lexer() = default;

std::vector<Tokens> Lexer::tokenization(CharStream& charStream, UseManager& useManager, Diagnostics& diagnostics)
{
    useManager.push(charStream);
    return this->tokenization(useManager, diagnostics);
}

std::vector<Tokens> Lexer::tokenization(UseManager& useManager, Diagnostics& diagnostics)
{
    this->_useManager = &useManager;

    std::vector<Tokens> vecTokens;

    while (!this->_useManager->empty())
    {
        if (vecTokens.empty())
            vecTokens.emplace_back(this->tokenization(this->_useManager->front(), diagnostics));
        else
            vecTokens.emplace(vecTokens.end() - 1, this->tokenization(this->_useManager->front(), diagnostics));

        this->_useManager->pop();
    }

    this->_useManager = nullptr;

    return vecTokens;
}

Tokens Lexer::tokenization(CharStream& charStream, Diagnostics& diagnostics)
{
    this->_diagnostics = &diagnostics;

    Tokens tokens{this->tokenization(charStream)};

    this->_diagnostics = nullptr;

    return tokens;
}

Tokens Lexer::tokenization(CharStream& charStream)
{
    this->_charStream = &charStream;

    Tokens tokens{this->_charStream, {}};

    auto& elements{tokens.elements};
    elements.emplace_back(this->_nextToken());
    auto* element{&elements.back()};

    while (this->_useManager != nullptr && element != nullptr && element->type == TokenType::KW_USE)
    {
        elements.pop_back();

        elements.emplace_back(this->_nextToken());
        element = &elements.back();

        if (element->type != TokenType::LIT_STR)
        {
            this->_diagnosticError(*(element - 1), "Can not be used.");
            continue;
        }

        while (element->type == TokenType::LIT_STR)
        {
            try
            {
                this->_useManager->push(element->lexeme.substr(1, element->lexeme.size() - 2).c_str());
            }
            catch (const std::exception& error)
            {
                this->_diagnosticError(*element, error.what());
            }

            elements.pop_back();

            elements.emplace_back(this->_nextToken());
            element = &elements.back();
        }
    }

    while (element != nullptr && element->type != TokenType::NEOF)
    {
        if (this->_useManager != nullptr && element->type == TokenType::KW_USE)
            this->_diagnosticError(*element, "Cannot use files in this area.");

        elements.emplace_back(this->_nextToken());
        element = &elements.back();
    }

    this->_charStream = nullptr;

    return tokens;
}

void Lexer::_diagnosticError(const Token& token, std::string message)
{
    if (this->_diagnostics == nullptr)
        return;

    this->_diagnostics->add({DiagnosticCategory::error, DiagnosticModule::Lexer, this->_charStream, {{token.line, token.column, token.lexeme.size()}}, std::move(message)});
}

Token Lexer::_nextToken()
{
    if (this->_charStream == nullptr)
        return {TokenType::NEOF, "", 0, 0};

    while (!this->_charStream->end())
    {
        if (this->_skipWs() || this->_skipComment())
            continue;

        Token token;
        if (this->_makeToken(token))
            return token;
        if (this->_makeTokenLitStr(token))
            return token;
        if (this->_makeTokenLitNum(token))
            return token;
        if (this->_makeTokenId(token))
            return token;

        token.type = TokenType::UNKNOWN;
        token.line = this->_charStream->line();
        token.column = this->_charStream->column();
        token.lexeme = this->_charStream->cchar();
        this->_diagnosticError(token, "Unexpected character.");
        this->_charStream->next();
        return token;
    }

    return {TokenType::NEOF, "", this->_charStream->line(), this->_charStream->column()};
}

bool Lexer::_skipWs()
{
    if (this->_charStream == nullptr || this->_charStream->end() || !std::isspace(this->_charStream->cchar()))
        return false;

    do
    {
        this->_charStream->next();
    } while (!this->_charStream->end() && std::isspace(this->_charStream->cchar()));

    return true;
}

bool Lexer::_skipComment()
{
    // Skip single-line comment "// ..."
    if (this->_charStream->cmatch("//"))
    {
        this->_charStream->next(2);

        while (!this->_charStream->end() && this->_charStream->cchar() != '\n')
            this->_charStream->next();

        return true;
    }

    this->_charStream->saveStateTemp();

    // Skip multi-line comment "/* ... */"
    if (this->_charStream->cmatch("/*"))
    {
        this->_charStream->next(2);

        while (!this->_charStream->end())
        {
            if (this->_charStream->cmatch("*/"))
            {
                this->_charStream->next(2);
                return true;
            }
            this->_charStream->next();
        }

        this->_charStream->loadStateTemp();
        return false;
    }

    return false;
}

bool Lexer::_makeToken(Token& token)
{
    static constexpr std::array<std::pair<TokenType, std::pair<const char*, bool>>, 12> tokenTypeWithPatterns{{
        // Parenthesis
        {TokenType::PAREN_OPEN, {"(", false}},
        {TokenType::PAREN_CLOSE, {")", false}},

        // Separators
        {TokenType::COMMA, {",", false}},

        // Data types
        {TokenType::KW_DT_NUM, {"num", true}},
        {TokenType::KW_DT_STR, {"str", true}},

        // Keywords
        {TokenType::KW_F, {"f", true}},
        {TokenType::KW_USE, {"use", true}},
    }};

    for (const auto& [type, patternPair] : tokenTypeWithPatterns)
    {
        const auto& [pattern, strict] = patternPair;

        if (pattern == nullptr)
            continue;

        if (!this->_charStream->cmatch(pattern))
            continue;

        const size_t size = std::strlen(pattern);

        if (strict)
        {
            size_t futureIndex = this->_charStream->index() + size;
            if (futureIndex < this->_charStream->size())
            {
                char futureChar = this->_charStream->charAt(futureIndex);
                if (std::isalnum(futureChar))
                    continue;
            }
        }

        token.type = type;
        token.lexeme = pattern;
        token.line = this->_charStream->line();
        token.column = this->_charStream->column();

        this->_charStream->next(size);

        return true;
    }

    return false;
}

bool Lexer::_makeTokenLitStr(Token& token)
{
    if (this->_charStream->end())
        return false;

    this->_charStream->saveStateTemp();

    char quote{this->_charStream->cchar()};
    if (quote != '"' && quote != '\'')
        return false;

    token.type = TokenType::LIT_STR;
    token.lexeme = quote;
    token.line = this->_charStream->line();
    token.column = this->_charStream->column();

    this->_charStream->next();

    while (!this->_charStream->end())
    {
        char c{this->_charStream->cchar()};

        if (c == quote)
        {
            token.lexeme += c;
            this->_charStream->next();
            return true;
        }

        token.lexeme += c;
        this->_charStream->next();
    }

    this->_charStream->loadStateTemp();
    return false;
}

bool Lexer::_makeTokenLitNum(Token& token)
{
    if (this->_charStream->end())
        return false;

    this->_charStream->saveStateTemp();

    bool hasDot{false};
    bool hasDigit{false};

    token.type = TokenType::LIT_NUM;
    token.lexeme = "";
    token.line = this->_charStream->line();
    token.column = this->_charStream->column();

    while (!this->_charStream->end())
    {
        char c{this->_charStream->cchar()};

        if (std::isdigit(c))
        {
            hasDigit = true;
            token.lexeme += c;
        }
        else if (c == '.' && !hasDot)
        {
            hasDot = true;
            token.lexeme += c;
        }
        else
        {
            break;
        }

        this->_charStream->next();
    }

    // If there is only a point without numbers, invalid
    if (!hasDigit)
    {
        this->_charStream->loadStateTemp();
        return false;
    }

    return true;
}

bool Lexer::_makeTokenId(Token& token)
{
    if (this->_charStream->end())
        return false;

    char c{this->_charStream->cchar()};

    // Identifier must begin with letters or underscore
    if (!std::isalpha(c) && c != '_')
        return false;

    token.type = TokenType::ID;
    token.lexeme.clear();
    token.line = this->_charStream->line();
    token.column = this->_charStream->column();

    // Add the first character
    token.lexeme += c;
    this->_charStream->next();

    // Loop to take the remaining identifier (letters, numbers, or underscore)
    while (!this->_charStream->end())
    {
        c = this->_charStream->cchar();

        if (!std::isalnum(c) && c != '_')
            break;

        token.lexeme += c;
        this->_charStream->next();
    }

    return true;
}

} // namespace nusa