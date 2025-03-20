/*
 * ----------------------------------------------------------------------------
 * Project: Nusa
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, The Nusa Project Authors
 * ----------------------------------------------------------------------------
 */

#ifndef NUSA_AST_AST_FUNCTION_H
#define NUSA_AST_AST_FUNCTION_H

#include "nusa/ast/ast.h"
#include "nusa/ast/ast_variable.h"
#include "nusa/lexer/token/token.h"
#include <vector>

namespace nusa {

struct ASTFunction : public AST
{
    Token* name{nullptr};
    std::vector<ASTVariable> parameters;
};

} // namespace nusa

#endif