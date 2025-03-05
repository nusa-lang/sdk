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
#include "nusantara/support/file/memory_mapped_file.h"
#include <cctype>
#include <cstdio>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace nusantara {

std::vector<std::pair<TokenType, std::string>> Lexer::_rules{
    {TokenType::PAREN_OPEN, "("},
    {TokenType::PAREN_CLOSE, ")"},
    {TokenType::KW_DT_STR, "teks"},
    {TokenType::KW_EXTERN, "luar"},
    {TokenType::KW_FUNC, "f"},
    {TokenType::KW_IMPRT, "impor"},
};

Lexer Lexer::file(const std::string& source)
{
    Lexer lexer;
    lexer._source = std::filesystem::weakly_canonical(source);
    lexer._file = MemoryMappedFile::create(lexer._source);
    lexer._size = lexer._file.value().size();
    return lexer;
}

Lexer Lexer::input(std::string input)
{
    Lexer lexer;
    lexer._input = std::move(input);
    lexer._size = lexer._input->size();
    return lexer;
}

Token Lexer::nextToken()
{
    while (this->_notEof())
    {
        if (_skipWs() || _skipComment())
            continue;

        Token token;
        for (const auto& basic : Lexer::_rules)
            if (this->_create(token, basic.first, basic.second))
                return token;

        if (this->_createLitStr(token))
            return token;
        if (this->_createIdentifier(token))
            return token;

        token.type = TokenType::UNKNOWN;
        token.line = this->_line;
        token.column = this->_column;
        token.lexeme = *this->_char();
        this->_next();
        return token;
    }

    return {TokenType::NEOF, "", this->_line, this->_column};
}

std::vector<Tokens> Lexer::getVecTokens()
{
    std::unordered_set<std::string> importedSources;

    std::vector<Tokens> vecToken;
    vecToken.reserve(10);

    Tokens tokens{this->_source, {}};
    auto& elements{tokens.elements};
    elements.reserve(100);

    elements.emplace_back(this->nextToken());
    auto* element{&elements.back()};

    importedSources.insert(this->_source);

    while (element != nullptr && element->type == TokenType::KW_IMPRT)
    {
        elements.emplace_back(this->nextToken());
        element = &elements.back();

        if (element->type != TokenType::LIT_STR)
            throw std::runtime_error("Hanya dapat meimpor teks.");

        std::string source{element->lexeme.substr(1, element->lexeme.size() - 2)};

        if (!importedSources.contains(source))
        {
            importedSources.insert(source);

            auto lexer{Lexer::file(source)};

            for (auto& importedTokens : lexer.getVecTokens())
                vecToken.emplace_back(std::move(importedTokens));
        }

        elements.emplace_back(this->nextToken());
        element = &elements.back();
    }

    while (element != nullptr && element->type != TokenType::NEOF)
    {
        if (element->type == TokenType::KW_IMPRT)
            throw std::runtime_error("Impor hanya dapat dilakukan di bagian atas.");

        elements.emplace_back(this->nextToken());
        element = &elements.back();
    }

    vecToken.emplace_back(std::move(tokens));

    return vecToken;
}

const char* Lexer::_char() const
{
    if (this->_eof())
        return nullptr;

    if (this->_input.has_value())
        return &this->_input.value()[this->_index];

    if (this->_file.has_value())
        return &this->_file.value().chars()[this->_index];

    return nullptr;
}

bool Lexer::_eof() const
{
    return this->_index >= this->_size;
}

bool Lexer::_notEof() const
{
    return this->_index < this->_size;
}

void Lexer::_next()
{
    const char* c{this->_char()};
    if (c == nullptr)
        return;

    if (*c == '\n')
    {
        this->_line++;
        this->_column = 0;
    }
    else
        this->_column++;

    this->_index++;
}

bool Lexer::_skipWs()
{
    const char* c{this->_char()};
    if (c == nullptr || std::isspace(*c) == 0)
        return false;

    while (c != nullptr && std::isspace(*c) != 0)
    {
        this->_next();
        c = this->_char();
    }

    return true;
}

bool Lexer::_skipComment()
{
    const char* c = this->_char();
    if (c == nullptr)
        return false;

    size_t tempIndex = this->_index;

    // Check for single-line comment
    if (c[0] == '/' && c[1] == '/')
    {
        this->_next(); // Skip the first '/'
        while ((c = this->_char()) != nullptr && *c != '\n')
        {
            this->_next();
        }
        return true;
    }

    // Check for multi-line comment
    if (c[0] == '/' && c[1] == '*')
    {
        this->_next(); // Skip the first '/'
        this->_next(); // Skip the second '*'
        while ((c = this->_char()) != nullptr)
        {
            if (*c == '*')
            {
                this->_next();
                c = this->_char();
                if (c != nullptr && *c == '/')
                {
                    this->_next(); // Skip '/'
                    return true;
                }
            }
            this->_next();
        }
    }

    this->_index = tempIndex;
    return false;
}

bool Lexer::_create(Token& token, const TokenType& type, const std::string& rule)
{
    size_t endIndex{rule.size() == 0 ? 0 : rule.size() - 1};
    if ((this->_index + endIndex) >= this->_size)
        return false;

    size_t tempIndex{this->_index};

    token.type = type;
    token.line = this->_line;
    token.column = this->_column;
    token.lexeme = "";

    for (size_t i{0}; i <= endIndex; i++)
    {
        const char* c{this->_char()};

        if (c == nullptr || *c != rule[i])
        {
            this->_index = tempIndex;
            this->_line = token.line;
            this->_column = token.column;
            return false;
        }

        token.lexeme += *c;
        this->_next();
    }

    return true;
}

bool Lexer::_createLitStr(Token& token)
{
    const char* c{this->_char()};
    if (c == nullptr || (*c != '"' && *c != '\''))
        return false;

    char quote{*c};

    token.type = TokenType::LIT_STR;
    token.lexeme = quote;
    token.line = this->_line;
    token.column = this->_column;

    this->_next();
    c = this->_char();

    while (c != nullptr && *c != quote)
    {
        token.lexeme += *c;
        this->_next();
        c = this->_char();
    }

    if (c != nullptr)
    {
        token.lexeme += *c;
        this->_next();
    }

    return true;
}

bool Lexer::_createIdentifier(Token& token)
{
    const char* c{this->_char()};
    if (c == nullptr || (std::isalpha(*c) == 0 && *c != '_'))
        return false;

    token.type = TokenType::IDENTIFIER;
    token.lexeme = *c;
    token.line = this->_line;
    token.column = this->_column;

    this->_next();
    c = this->_char();

    while (c != nullptr && (std::isalnum(*c) != 0 || *c == '_'))
    {
        token.lexeme += *c;
        this->_next();
        c = this->_char();
    }

    return true;
}

} // namespace nusantara