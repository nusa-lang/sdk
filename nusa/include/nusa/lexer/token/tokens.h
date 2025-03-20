/*
 * ----------------------------------------------------------------------------
 * Project: Nusa
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, The Nusa Project Authors
 * ----------------------------------------------------------------------------
 */

#ifndef NUSA_LEXER_TOKEN_TOKENS_H
#define NUSA_LEXER_TOKEN_TOKENS_H

#include "nusa/lexer/token/token.h"
#include "nusa/support/char_stream.h"
#include "nusa/support/out_stream.h"
#include <vector>

namespace nusa {

struct Tokens
{
    CharStream* charStream{nullptr};
    std::vector<Token> elements;
};

OutStream& operator<<(OutStream& os, const Tokens& tokens);

} // namespace nusa

#endif