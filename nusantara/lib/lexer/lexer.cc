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
#include "nusantara/support/log/log.h"
#include <cctype>
#include <cstdio>
#include <exception>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace nusantara {

std::vector<std::pair<TokenType, std::string>> Lexer::_rules{
    {TokenType::PAREN_OPEN, "("},
    {TokenType::PAREN_CLOSE, ")"},
    {TokenType::KW_DT_STR, "teks"},
    {TokenType::KW_EXTERN, "luar"},
    {TokenType::KW_FUNC, "f"},
    {TokenType::KW_INCLUDE, "muat"},
};

std::vector<Tokens> Lexer::file(std::string source)
{
    source = std::filesystem::weakly_canonical(source);

    if (this->_files.contains(source))
        return {};

    return this->_loadTokens(Lexer::_createData(*this->_files.emplace(std::move(source)).first, true));
}

std::vector<Tokens> Lexer::input(std::string input)
{
    return this->_loadTokens(Lexer::_createData(std::move(input), false));
}

Lexer::Data Lexer::_createData(std::string source, const bool& file)
{
    Data data;

    if (file)
    {
        data.source = std::move(source);
        data.file = MemoryMappedFile::create(data.source);
        data.size = data.file.value().size();
    }
    else
    {
        data.size = source.size();
        data.input = std::move(source);
    }

    return data;
}

Token Lexer::_nextToken(Data& data)
{
    while (Lexer::_notEof(data))
    {
        if (_skipWs(data) || _skipComment(data))
            continue;

        Token token;
        for (const auto& basic : Lexer::_rules)
            if (Lexer::_create(data, token, basic.first, basic.second))
                return token;

        if (Lexer::_createLitStr(data, token))
            return token;
        if (Lexer::_createIdentifier(data, token))
            return token;

        token.type = TokenType::UNKNOWN;
        token.line = data.line;
        token.column = data.column;
        token.lexeme = *Lexer::_char(data);
        Log::saveError(data.source, token.line, token.column, token.lexeme.size(), "[L] Tidak dikenal.");
        Lexer::_next(data);
        return token;
    }

    return {TokenType::NEOF, "", data.line, data.column};
}

const char* Lexer::_char(Data& data)
{
    return Lexer::_eof(data) ? nullptr : data.input ? &data.input.value()[data.index] : data.file ? &data.file.value().chars()[data.index] : nullptr;
}

bool Lexer::_eof(Data& data)
{
    return data.index >= data.size;
}

bool Lexer::_notEof(Data& data)
{
    return data.index < data.size;
}

void Lexer::_next(Data& data)
{
    const char* c{Lexer::_char(data)};
    if (c == nullptr)
        return;

    if (*c == '\n')
    {
        data.line++;
        data.column = 0;
    }
    else
        data.column++;

    data.index++;
}

bool Lexer::_skipWs(Data& data)
{
    const char* c{Lexer::_char(data)};
    if (c == nullptr || std::isspace(*c) == 0)
        return false;

    while (c != nullptr && std::isspace(*c) != 0)
    {
        Lexer::_next(data);
        c = Lexer::_char(data);
    }

    return true;
}

bool Lexer::_skipComment(Data& data)
{
    const char* c = Lexer::_char(data);
    if (c == nullptr)
        return false;

    size_t tempIndex = data.index;

    // Check for single-line comment
    if (c[0] == '/' && c[1] == '/')
    {
        Lexer::_next(data); // Skip the first '/'
        while ((c = Lexer::_char(data)) != nullptr && *c != '\n')
        {
            Lexer::_next(data);
        }
        return true;
    }

    // Check for multi-line comment
    if (c[0] == '/' && c[1] == '*')
    {
        Lexer::_next(data); // Skip the first '/'
        Lexer::_next(data); // Skip the second '*'
        while ((c = Lexer::_char(data)) != nullptr)
        {
            if (*c == '*')
            {
                Lexer::_next(data);
                c = Lexer::_char(data);
                if (c != nullptr && *c == '/')
                {
                    Lexer::_next(data); // Skip '/'
                    return true;
                }
            }
            Lexer::_next(data);
        }
    }

    data.index = tempIndex;
    return false;
}

bool Lexer::_create(Data& data, Token& token, const TokenType& type, std::string_view rule)
{
    size_t endIndex{rule.size() == 0 ? 0 : rule.size() - 1};
    if (data.index + rule.size() > data.size)
        return false;

    size_t tempIndex{data.index};

    token.type = type;
    token.line = data.line;
    token.column = data.column;
    token.lexeme = "";

    for (size_t i{0}; i <= endIndex; i++)
    {
        const char* c{Lexer::_char(data)};

        if (c == nullptr || *c != rule[i])
        {
            data.index = tempIndex;
            data.line = token.line;
            data.column = token.column;
            return false;
        }

        token.lexeme += *c;
        Lexer::_next(data);
    }

    return true;
}

bool Lexer::_createLitStr(Data& data, Token& token)
{
    const char* c{Lexer::_char(data)};
    if (c == nullptr || (*c != '"' && *c != '\''))
        return false;

    char quote{*c};

    token.type = TokenType::LIT_STR;
    token.lexeme = quote;
    token.line = data.line;
    token.column = data.column;

    Lexer::_next(data);
    c = Lexer::_char(data);

    while (c != nullptr && *c != quote)
    {
        token.lexeme += *c;
        Lexer::_next(data);
        c = Lexer::_char(data);
    }

    if (c != nullptr)
    {
        token.lexeme += *c;
        Lexer::_next(data);
    }

    return true;
}

bool Lexer::_createIdentifier(Data& data, Token& token)
{
    const char* c{Lexer::_char(data)};
    if (c == nullptr || (std::isalpha(*c) == 0 && *c != '_'))
        return false;

    token.type = TokenType::IDENTIFIER;
    token.lexeme = *c;
    token.line = data.line;
    token.column = data.column;

    Lexer::_next(data);
    c = Lexer::_char(data);

    while (c != nullptr && (std::isalnum(*c) != 0 || *c == '_'))
    {
        token.lexeme += *c;
        Lexer::_next(data);
        c = Lexer::_char(data);
    }

    return true;
}

std::vector<Tokens> Lexer::_loadTokens(Data data)
{
    std::vector<Tokens> vecTokens;

    Tokens tokens{data.source, {}};
    auto& elements{tokens.elements};

    elements.emplace_back(Lexer::_nextToken(data));
    auto* element{&elements.back()};

    while (element != nullptr && element->type == TokenType::KW_INCLUDE)
    {
        elements.emplace_back(Lexer::_nextToken(data));
        element = &elements.back();

        if (element->type != TokenType::LIT_STR)
        {
            Log::saveError(data.source, element->line, element->column, element->lexeme.size(), "[L] Tidak bisa dimuat.");

            elements.emplace_back(Lexer::_nextToken(data));
            element = &elements.back();

            continue;
        }

        std::string source{std::filesystem::weakly_canonical(element->lexeme.substr(1, element->lexeme.size() - 2))};

        if (!this->_files.contains(source))
        {
            try
            {
                this->_report = false;
                std::vector<Tokens> importedVecTokens{this->file(source)};
                this->_report = true;

                for (auto& importedTokens : importedVecTokens)
                    vecTokens.emplace_back(std::move(importedTokens));
            }
            catch (const std::exception& error)
            {
                Log::saveError(data.source, element->line, element->column, element->lexeme.size(), "[L] " + (std::string)error.what());
            }
        }

        elements.emplace_back(Lexer::_nextToken(data));
        element = &elements.back();
    }

    while (element != nullptr && element->type != TokenType::NEOF)
    {
        if (element->type == TokenType::KW_INCLUDE)
            Log::saveError(data.source, element->line, element->column, element->lexeme.size(), "[L] Tidak dapat memuat file di area ini.");

        elements.emplace_back(Lexer::_nextToken(data));
        element = &elements.back();
    }

    vecTokens.emplace_back(std::move(tokens));

    if (this->_report)
        Log::report();

    return vecTokens;
}

} // namespace nusantara