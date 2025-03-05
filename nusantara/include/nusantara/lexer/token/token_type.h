/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_LEXER_TOKEN_TYPE_H
#define NUSANTARA_LEXER_TOKEN_TYPE_H

#include <llvm/Support/raw_ostream.h>

namespace nusantara {

enum class TokenType
{
    // Parenthesis
    PAREN_OPEN,  // pattern: '('
    PAREN_CLOSE, // pattern: ')'

    // Data types
    KW_DT_STR, // pattern: 'teks'

    // Keywords
    KW_EXTERN, // pattern: 'luar'
    KW_FUNC,   // pattern: 'f'
    KW_IMPRT,  // pattern: 'impor'

    // Literals
    LIT_STR, // pattern: ('\'' ~'\''* '\'' | '"' ~'"'* '"')

    // System
    IDENTIFIER, // pattern: [a-zA-Z_][a-zA-Z0-9_]*
    UNKNOWN,    // unknown
    NEOF,       // eof
};

llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const TokenType& type);

} // namespace nusantara

#endif