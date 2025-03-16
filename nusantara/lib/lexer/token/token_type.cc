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
#include <string_view>

namespace nusantara {

llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const TokenType& type)
{
    std::string_view name{magic_enum::enum_name(type)};
    os.write(name.data(), name.size());
    return os;
}

} // namespace nusantara