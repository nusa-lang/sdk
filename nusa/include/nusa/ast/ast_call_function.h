/*
 * ----------------------------------------------------------------------------
 * Project: Nusa
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, The Nusa Project Authors
 * ----------------------------------------------------------------------------
 */

#ifndef NUSA_AST_AST_CALL_FUNCTION_H
#define NUSA_AST_AST_CALL_FUNCTION_H

#include "nusa/ast/ast.h"
#include "nusa/lexer/token/token.h"
#include <memory>
#include <vector>

namespace nusa {

struct ASTCallFunction : public AST
{
    Token* name{nullptr};
    std::vector<std::unique_ptr<AST>> arguments;
};

} // namespace nusa

#endif