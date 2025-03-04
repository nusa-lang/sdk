/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/lexer/token/token.h"
#include <llvm/Support/raw_ostream.h>

namespace nusantara {

llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const Token& token)
{
    os << token.line << ":" << token.column << " [" << token.type << "] " << token.lexeme;
    return os;
}

} // namespace nusantara