/*
 * ----------------------------------------------------------------------------
 * Project: Nusa
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, The Nusa Project Authors
 * ----------------------------------------------------------------------------
 */

#include "nusa/lexer/token/token_type.h"
#include "nusa/support/out_stream.h"
#include <magic_enum.hpp>

namespace nusa {

OutStream& operator<<(OutStream& os, const TokenType& type)
{
    os << magic_enum::enum_name(type);
    return os;
}

} // namespace nusa