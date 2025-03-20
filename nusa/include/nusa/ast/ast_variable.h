/*
 * ----------------------------------------------------------------------------
 * Project: Nusa
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, The Nusa Project Authors
 * ----------------------------------------------------------------------------
 */

#ifndef NUSA_AST_AST_VARIABLE_H
#define NUSA_AST_AST_VARIABLE_H

#include "nusa/ast/ast.h"
#include "nusa/lexer/token/token.h"

namespace nusa {

struct ASTVariable : public AST
{
    Token* type{nullptr};
    Token* name{nullptr};
};

} // namespace nusa

#endif