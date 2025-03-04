/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/lexer/token/tokens.h"
#include <llvm/Support/raw_ostream.h>

namespace nusantara {

llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const Tokens& tokens)
{
    for (const auto& element : tokens.elements)
        os << tokens.source << ":" << element << "\n";

    return os;
}

} // namespace nusantara