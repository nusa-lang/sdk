/*
 * ----------------------------------------------------------------------------
 * Project: Nusa
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, The Nusa Project Authors
 * ----------------------------------------------------------------------------
 */

#ifndef NUSA_PARSER_PARSER_H
#define NUSA_PARSER_PARSER_H

#include "nusa/ast/ast.h"
#include "nusa/ast/asts.h"
#include "nusa/lexer/token/token.h"
#include "nusa/lexer/token/token_type.h"
#include "nusa/lexer/token/tokens.h"
#include "nusa/support/diagnostic/diagnostic.h"
#include "nusa/support/diagnostic/diagnostics.h"
#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

namespace nusa {

class Parser
{
public:
    Parser();

    std::vector<ASTS> parse(std::vector<Tokens>& vecTokens, Diagnostics& diagnostics);
    ASTS parse(Tokens& tokens, Diagnostics& diagnostics);

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

    std::unique_ptr<AST> _parseFunction();
    std::unique_ptr<AST> _parsePrimaryExpression();
};

} // namespace nusa

#endif