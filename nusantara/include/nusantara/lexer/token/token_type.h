/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_LEXER_TOKEN_TOKEN_TYPE_H
#define NUSANTARA_LEXER_TOKEN_TOKEN_TYPE_H

#include "nusantara/support/out_stream.h"

namespace nusantara {

enum class TokenType
{
    // Parenthesis
    PAREN_OPEN,  // pattern: '('
    PAREN_CLOSE, // pattern: ')'

    // Separators
    COMMA, // pattern: ','

    // Data types
    KW_DT_I1,  // pattern: 'i1'
    KW_DT_I8,  // pattern: 'i8'
    KW_DT_I16, // pattern: 'i16'
    KW_DT_I32, // pattern: 'i32'
    KW_DT_I64, // pattern: 'i64'

    KW_DT_F32, // pattern: 'f32'
    KW_DT_F64, // pattern: 'f64'

    // Keywords
    KW_F,   // pattern: 'f'
    KW_USE, // pattern: 'use'

    // Literals
    LIT_STR, // pattern: ('\'' ~'\''* '\'' | '"' ~'"'* '"')
    LIT_NUM, // pattern: [0-9]+ ('.' [0-9]+)?

    // System
    ID,      // pattern: [a-zA-Z_][a-zA-Z0-9_]*
    UNKNOWN, // unknown
    NEOF,    // eof
};

OutStream& operator<<(OutStream& os, const TokenType& type);

} // namespace nusantara

#endif