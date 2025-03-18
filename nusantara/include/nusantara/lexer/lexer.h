/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_LEXER_LEXER_H
#define NUSANTARA_LEXER_LEXER_H

#include "nusantara/lexer/token/token.h"
#include "nusantara/lexer/token/tokens.h"
#include "nusantara/lexer/use_manager.h"
#include "nusantara/support/char_stream.h"
#include "nusantara/support/diagnostic/diagnostics.h"
#include <string>
#include <vector>

namespace nusantara {

class Lexer
{
public:
    Lexer();

    std::vector<Tokens> tokenization(CharStream& charStream, UseManager& moduleManager, Diagnostics& diagnostics);
    std::vector<Tokens> tokenization(UseManager& moduleManager, Diagnostics& diagnostics);
    Tokens tokenization(CharStream& charStream, Diagnostics& diagnostics);
    Tokens tokenization(CharStream& charStream);

private:
    Diagnostics* _diagnostics{nullptr};
    UseManager* _moduleManager{nullptr};
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

} // namespace nusantara

#endif