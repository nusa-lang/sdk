/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_AST_AST_VARIABLE_H
#define NUSANTARA_AST_AST_VARIABLE_H

#include "nusantara/ast/ast.h"
#include "nusantara/lexer/token/token.h"

namespace nusantara {

struct ASTVariable : public AST
{
    Token* type{nullptr};
    Token* name{nullptr};
};

} // namespace nusantara

#endif