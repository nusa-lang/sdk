/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_LEXER_H
#define NUSANTARA_LEXER_H

#include "nusantara/lexer/token/token.h"
#include "nusantara/lexer/token/token_type.h"
#include "nusantara/lexer/token/tokens.h"
#include "nusantara/support/file/memory_mapped_file.h"
#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_set>
#include <utility>
#include <vector>

namespace nusantara {

class Lexer
{
public:
    Lexer() = default;
    std::vector<Tokens> file(std::string source);
    std::vector<Tokens> input(std::string input);

private:
    struct Data
    {
        std::string source{"unknown"};

        size_t size{0};
        size_t index{0};
        size_t line{0};
        size_t column{0};

        std::optional<std::string> input{std::nullopt};
        std::optional<MemoryMappedFile> file{std::nullopt};
    };

    bool _report{true};

    std::unordered_set<std::string> _files;

    static std::vector<std::pair<TokenType, std::string>> _rules;

    static Data _createData(std::string source, const bool& file);

    static Token _nextToken(Data& data);

    [[nodiscard]] static const char* _char(Data& data);

    [[nodiscard]] static bool _eof(Data& data);
    [[nodiscard]] static bool _notEof(Data& data);

    static void _next(Data& data);

    static bool _skipWs(Data& data);

    static bool _skipComment(Data& data);

    static bool _create(Data& data, Token& token, const TokenType& type, std::string_view rule);

    static bool _createLitStr(Data& data, Token& token);

    static bool _createIdentifier(Data& data, Token& token);

    std::vector<Tokens> _loadTokens(Data data);
};

} // namespace nusantara

#endif