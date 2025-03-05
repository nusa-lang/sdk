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
#include <utility>
#include <vector>

namespace nusantara {

class Lexer
{
public:
    static Lexer file(const std::string& source);
    static Lexer input(std::string input);

    Token nextToken();

    std::vector<Tokens> getVecTokens();

private:
    Lexer() = default;

    static std::vector<std::pair<TokenType, std::string>> _rules;

    std::string _source{"unknown"};

    std::optional<std::string> _input{std::nullopt};
    std::optional<MemoryMappedFile> _file{std::nullopt};
    size_t _size{0};

    size_t _index{0};

    size_t _line{0};
    size_t _column{0};

    [[nodiscard]] const char* _char() const;

    [[nodiscard]] bool _eof() const;
    [[nodiscard]] bool _notEof() const;

    void _next();

    bool _skipWs();

    bool _skipComment();

    bool _create(Token& token, const TokenType& type, const std::string& rule);

    bool _createLitStr(Token& token);

    bool _createIdentifier(Token& token);
};

} // namespace nusantara

#endif