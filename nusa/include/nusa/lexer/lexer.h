/*
 * ----------------------------------------------------------------------------
 * Project: Nusa
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, The Nusa Project Authors
 * ----------------------------------------------------------------------------
 */

#ifndef NUSA_LEXER_LEXER_H
#define NUSA_LEXER_LEXER_H

#include "nusa/lexer/token/token.h"
#include "nusa/lexer/token/tokens.h"
#include "nusa/lexer/use_manager.h"
#include "nusa/support/char_stream.h"
#include "nusa/support/diagnostic/diagnostics.h"
#include <string>
#include <vector>

namespace nusa {

class Lexer
{
public:
    Lexer();

    std::vector<Tokens> tokenization(CharStream& charStream, UseManager& useManager, Diagnostics& diagnostics);
    std::vector<Tokens> tokenization(UseManager& useManager, Diagnostics& diagnostics);
    Tokens tokenization(CharStream& charStream, Diagnostics& diagnostics);
    Tokens tokenization(CharStream& charStream);

private:
    Diagnostics* _diagnostics{nullptr};
    UseManager* _useManager{nullptr};
    CharStream* _charStream{nullptr};

    void _diagnosticError(const Token& token, std::string message);

    Token _nextToken();

    bool _skipWs();
    bool _skipComment();

    bool _makeToken(Token& token);
    bool _makeTokenLitStr(Token& token);
    bool _makeTokenLitNum(Token& token);
    bool _makeTokenId(Token& token);
};

} // namespace nusa

#endif