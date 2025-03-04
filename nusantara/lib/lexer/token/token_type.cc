/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/lexer/token/token_type.h"
#include <llvm/Support/raw_ostream.h>
#include <magic_enum.hpp>

namespace nusantara {

llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const TokenType& type)
{
    os << magic_enum::enum_name(type);
    return os;
}

} // namespace nusantara