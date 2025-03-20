/*
 * ----------------------------------------------------------------------------
 * Project: Nusa
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, The Nusa Project Authors
 * ----------------------------------------------------------------------------
 */

#ifndef NUSA_LEXER_TOKEN_TOKEN_H
#define NUSA_LEXER_TOKEN_TOKEN_H

#include "nusa/lexer/token/token_type.h"
#include "nusa/support/out_stream.h"
#include <cstddef>
#include <string>

namespace nusa {

struct Token
{
    TokenType type{TokenType::UNKNOWN};
    std::string lexeme;
    size_t line{0};
    size_t column{0};
};

OutStream& operator<<(OutStream& os, const Token& token);

} // namespace nusa

#endif