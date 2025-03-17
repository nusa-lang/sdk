/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_LEXER_TOKEN_TOKEN_H
#define NUSANTARA_LEXER_TOKEN_TOKEN_H

#include "nusantara/lexer/token/token_type.h"
#include "nusantara/support/out_stream.h"
#include <cstddef>
#include <string>

namespace nusantara {

struct Token
{
    TokenType type{TokenType::UNKNOWN};
    std::string lexeme;
    size_t line{0};
    size_t column{0};
};

OutStream& operator<<(OutStream& os, const Token& token);

} // namespace nusantara

#endif