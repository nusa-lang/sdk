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
#include "nusantara/module/module_manager.h"
#include "nusantara/support/input_stream.h"
#include <array>
#include <cctype>
#include <cstddef>
#include <cstring>
#include <llvm/Support/raw_ostream.h>
#include <string>
#include <utility>
#include <vector>

namespace nusantara {

Lexer::Lexer() = default;

std::vector<Tokens> Lexer::tokenization(ModuleManager& moduleManager)
{
    this->_moduleManager = &moduleManager;

    std::vector<Tokens> vecTokens;

    while (!this->_moduleManager->empty())
    {
        if (vecTokens.empty())
            vecTokens.emplace_back(this->_input(this->_moduleManager->front()));
        else
            vecTokens.emplace(vecTokens.end() - 1, this->_input(this->_moduleManager->front()));

        this->_moduleManager->pop();
    }

    this->_moduleManager = nullptr;

    return vecTokens;
}

Tokens Lexer::_input(InputStream& inputStream)
{
    this->_inputStream = &inputStream;

    Tokens tokens{this->_inputStream, {}};

    auto& elements{tokens.elements};
    elements.emplace_back(this->_nextToken());
    auto* element{&elements.back()};

    while (element != nullptr && element->type == TokenType::KW_MODULE)
    {
        elements.emplace_back(this->_nextToken());
        element = &elements.back();

        if (element->type != TokenType::LIT_STR)
        {
            llvm::errs() << "Tidak bisa dimuat.\n";
            continue;
        }

        this->_moduleManager->push(element->lexeme.substr(1, element->lexeme.size() - 2));

        elements.emplace_back(this->_nextToken());
        element = &elements.back();
    }

    while (element != nullptr && element->type != TokenType::NEOF)
    {
        if (element->type == TokenType::KW_MODULE)
            llvm::errs() << "Tidak dapat memuat file di area ini.\n";

        elements.emplace_back(this->_nextToken());
        element = &elements.back();
    }

    this->_inputStream = nullptr;

    return tokens;
}

Token Lexer::_nextToken()
{
    if (this->_inputStream == nullptr)
        return {TokenType::NEOF, "", 0, 0};

    while (!this->_inputStream->end())
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
        token.line = this->_inputStream->line();
        token.column = this->_inputStream->column();
        token.lexeme = this->_inputStream->cchar();
        this->_inputStream->next();
        return token;
    }

    return {TokenType::NEOF, "", this->_inputStream->line(), this->_inputStream->column()};
}

bool Lexer::_skipWs()
{
    if (this->_inputStream == nullptr || this->_inputStream->end() || !std::isspace(this->_inputStream->cchar()))
        return false;

    do
    {
        this->_inputStream->next();
    } while (!this->_inputStream->end() && std::isspace(this->_inputStream->cchar()));

    return true;
}

bool Lexer::_skipComment()
{
    // Skip single-line comment "// ..."
    if (this->_inputStream->cmatch("//"))
    {
        this->_inputStream->next(2);

        while (!this->_inputStream->end() && this->_inputStream->cchar() != '\n')
            this->_inputStream->next();

        return true;
    }

    this->_inputStream->save();

    // Skip multi-line comment "/* ... */"
    if (this->_inputStream->cmatch("/*"))
    {
        this->_inputStream->next(2);

        while (!this->_inputStream->end())
        {
            if (this->_inputStream->cmatch("*/"))
            {
                this->_inputStream->next(2);
                return true;
            }
            this->_inputStream->next();
        }

        this->_inputStream->load();
        return false;
    }

    return false;
}

bool Lexer::_makeToken(Token& token)
{
    static constexpr std::array<std::pair<TokenType, std::pair<const char*, bool>>, 12> tokenTypeWithPatterns{{
        // Parenthesis
        {TokenType::PAREN_OPEN, {"(", false}},

        // Separators
        {TokenType::PAREN_CLOSE, {")", false}},
        {TokenType::PAREN_CLOSE, {",", false}},

        // Data types
        {TokenType::KW_DT_I1, {"b1", true}},
        {TokenType::KW_DT_I8, {"b8", true}},
        {TokenType::KW_DT_I16, {"b16", true}},
        {TokenType::KW_DT_I32, {"b32", true}},
        {TokenType::KW_DT_I64, {"b64", true}},

        {TokenType::KW_DT_F32, {"d32", true}},
        {TokenType::KW_DT_F64, {"d64", true}},

        // Keywords
        {TokenType::KW_FUNC, {"f", true}},
        {TokenType::KW_MODULE, {"muat", true}},
    }};

    for (const auto& [type, patternPair] : tokenTypeWithPatterns)
    {
        const auto& [pattern, strict] = patternPair;

        if (pattern == nullptr)
            continue;

        if (!this->_inputStream->cmatch(pattern))
            continue;

        const size_t size = std::strlen(pattern);

        if (strict)
        {
            size_t futureIndex = this->_inputStream->index() + size;
            if (futureIndex < this->_inputStream->size())
            {
                char futureChar = this->_inputStream->charAt(futureIndex);
                if (std::isalnum(futureChar))
                    continue;
            }
        }

        token.type = type;
        token.lexeme = pattern;
        token.line = this->_inputStream->line();
        token.column = this->_inputStream->column();

        this->_inputStream->next(size);

        return true;
    }

    return false;
}

bool Lexer::_makeTokenLitStr(Token& token)
{
    if (this->_inputStream->end())
        return false;

    this->_inputStream->save();

    char quote{this->_inputStream->cchar()};
    if (quote != '"' && quote != '\'')
        return false;

    token.type = TokenType::LIT_STR;
    token.lexeme = quote;
    token.line = this->_inputStream->line();
    token.column = this->_inputStream->column();

    this->_inputStream->next();

    while (!this->_inputStream->end())
    {
        char c{this->_inputStream->cchar()};

        if (c == quote)
        {
            token.lexeme += c;
            this->_inputStream->next();
            return true;
        }

        token.lexeme += c;
        this->_inputStream->next();
    }

    this->_inputStream->load();
    return false;
}

bool Lexer::_makeTokenLitNum(Token& token)
{
    if (this->_inputStream->end())
        return false;

    this->_inputStream->save();

    bool hasDot{false};
    bool hasDigit{false};

    token.type = TokenType::LIT_NUM;
    token.lexeme = "";
    token.line = this->_inputStream->line();
    token.column = this->_inputStream->column();

    while (!this->_inputStream->end())
    {
        char c{this->_inputStream->cchar()};

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

        this->_inputStream->next();
    }

    // If there is only a point without numbers, invalid
    if (!hasDigit)
    {
        this->_inputStream->load();
        return false;
    }

    return true;
}

bool Lexer::_makeTokenId(Token& token)
{
    if (this->_inputStream->end())
        return false;

    char c{this->_inputStream->cchar()};

    // Identifier must begin with letters or underscore
    if (!std::isalpha(c) && c != '_')
        return false;

    token.type = TokenType::ID;
    token.lexeme.clear();
    token.line = this->_inputStream->line();
    token.column = this->_inputStream->column();

    // Add the first character
    token.lexeme += c;
    this->_inputStream->next();

    // Loop to take the remaining identifier (letters, numbers, or underscore)
    while (!this->_inputStream->end())
    {
        c = this->_inputStream->cchar();

        if (!std::isalnum(c) && c != '_')
            break;

        token.lexeme += c;
        this->_inputStream->next();
    }

    return true;
}

} // namespace nusantara