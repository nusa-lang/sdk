/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_AST_AST_FUNCTION_H
#define NUSANTARA_AST_AST_FUNCTION_H

#include "nusantara/ast/ast.h"
#include "nusantara/ast/ast_variable.h"
#include "nusantara/lexer/token/token.h"
#include <vector>

namespace nusantara {

struct ASTFunction : public AST
{
    Token* name{nullptr};
    std::vector<ASTVariable> parameters;
};

} // namespace nusantara

#endif