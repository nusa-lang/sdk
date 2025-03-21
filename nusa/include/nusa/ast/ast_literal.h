/*
 * ----------------------------------------------------------------------------
 * Project: Nusa
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, The Nusa Project Authors
 * ----------------------------------------------------------------------------
 */

#ifndef NUSA_AST_AST_LITERAL_H
#define NUSA_AST_AST_LITERAL_H

#include "nusa/ast/ast.h"
#include "nusa/lexer/token/token.h"
#include <cstdint>
#include <string>
#include <variant>

namespace nusa {

struct ASTLiteral : public AST
{
    Token* token{nullptr};
    std::variant<std::monostate, int8_t, int16_t, int32_t, int64_t, float, double, std::string> value;
};

} // namespace nusa

#endif