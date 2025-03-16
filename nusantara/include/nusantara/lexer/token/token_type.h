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

#include <llvm/Support/raw_ostream.h>

namespace nusantara {

enum class TokenType
{
    // Parenthesis
    PAREN_OPEN,  // pattern: '('
    PAREN_CLOSE, // pattern: ')'

    // Separators
    COMMA, // pattern: ','

    // Data types
    KW_DT_I1,  // pattern: 'b1'
    KW_DT_I8,  // pattern: 'b8'
    KW_DT_I16, // pattern: 'b16'
    KW_DT_I32, // pattern: 'b32'
    KW_DT_I64, // pattern: 'b64'

    KW_DT_F32, // pattern: 'd32'
    KW_DT_F64, // pattern: 'd64'

    // Keywords
    KW_FUNC,   // pattern: 'f'
    KW_MODULE, // pattern: 'muat'

    // Literals
    LIT_STR, // pattern: ('\'' ~'\''* '\'' | '"' ~'"'* '"')
    LIT_NUM, // pattern: [0-9]+ ('.' [0-9]+)?

    // System
    ID,      // pattern: [a-zA-Z_][a-zA-Z0-9_]*
    UNKNOWN, // unknown
    NEOF,    // eof
};

llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const TokenType& type);

} // namespace nusantara

#endif