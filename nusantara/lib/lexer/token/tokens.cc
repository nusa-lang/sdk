/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/lexer/token/tokens.h"
#include <cstddef>
#include <llvm/Support/raw_ostream.h>

namespace nusantara {

llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const Tokens& tokens)
{
    const size_t& size{tokens.elements.size()};
    const size_t& sizeFrom0{size - 1};
    for (size_t i{0}; i < size; i++)
    {
        os << tokens.source << ":" << tokens.elements[i];
        if (i < sizeFrom0)
            os << '\n';
    }

    return os;
}

} // namespace nusantara