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
    // Instructions
    INST_PRINT, // pattern: cetak

    // Literals
    LIT_STR, // pattern: ('\'' ~'\''* '\'' | '"' ~'"'* '"')

    // System
    UNKNOWN, // unknown
    NEOF,    // eof
};

llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const TokenType& type);

} // namespace nusantara

#endif