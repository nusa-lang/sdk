/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_PARSER_PARSER_H
#define NUSANTARA_PARSER_PARSER_H

#include "nusantara/lexer/token/token.h"
#include "nusantara/lexer/token/token_type.h"
#include "nusantara/lexer/token/tokens.h"
#include "nusantara/support/diagnostic/diagnostic.h"
#include "nusantara/support/diagnostic/diagnostics.h"
#include <cstddef>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

namespace nusantara {

class Parser
{
public:
    Parser();

    void parse(std::vector<Tokens>& vecTokens, Diagnostics& diagnostics);
    void parse(Tokens& tokens, Diagnostics& diagnostics);

private:
    static std::unordered_set<TokenType> _dataTypes;

    Diagnostics* _diagnostics{nullptr};
    Tokens* _tokens{nullptr};
    Token* _token{nullptr};

    size_t _index{0};

    bool _eof();
    bool _notEof();

    bool _canNextIndex();
    void _nextIndex();

    bool _is(const TokenType& type);
    bool _is(const std::unordered_set<TokenType>& types);

    void _match(const TokenType& type, const std::optional<std::string>& errMsg = std::nullopt);
    void _match(const std::unordered_set<TokenType>& types, const std::optional<std::string>& errMsg = std::nullopt);

    std::optional<Diagnostic> _diagnosticError(std::string message);

    void _parseFunction();
    void _parseExpression();
};

} // namespace nusantara

#endif