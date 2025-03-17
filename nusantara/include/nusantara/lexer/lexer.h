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
#include "nusantara/module/module_manager.h"
#include "nusantara/support/diagnostic/diagnostics.h"
#include "nusantara/support/input_stream.h"
#include <string>
#include <vector>

namespace nusantara {

class Lexer
{
public:
    Lexer();

    std::vector<Tokens> tokenization(ModuleManager& moduleManager, Diagnostics& diagnostics);

private:
    Diagnostics* _diagnostics{nullptr};
    ModuleManager* _moduleManager{nullptr};
    InputStream* _inputStream{nullptr};

    void _diagnosticError(const Token& token, std::string message);

    Tokens _input(InputStream& inputStream);

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