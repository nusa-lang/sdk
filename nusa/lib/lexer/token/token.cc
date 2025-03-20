/*
 * ----------------------------------------------------------------------------
 * Project: Nusa
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, The Nusa Project Authors
 * ----------------------------------------------------------------------------
 */

#include "nusa/lexer/token/token.h"
#include "nusa/support/out_stream.h"

namespace nusa {

OutStream& operator<<(OutStream& os, const Token& token)
{
    os << token.line + 1 << ':' << token.column + 1 << " [" << token.type << "] " << token.lexeme;
    return os;
}

} // namespace nusa