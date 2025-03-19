/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_AST_AST_LITERAL_H
#define NUSANTARA_AST_AST_LITERAL_H

#include "nusantara/ast/ast.h"
#include "nusantara/lexer/token/token.h"
#include <cstdint>
#include <string>
#include <variant>

namespace nusantara {

struct ASTLiteral : public AST
{
    Token* token{nullptr};
    std::variant<std::monostate, int8_t, int16_t, int32_t, int64_t, float, double, std::string> value;
};

} // namespace nusantara

#endif