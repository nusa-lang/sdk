/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_AST_AST_CALL_FUNCTION_H
#define NUSANTARA_AST_AST_CALL_FUNCTION_H

#include "nusantara/ast/ast.h"
#include "nusantara/lexer/token/token.h"
#include <memory>
#include <vector>

namespace nusantara {

struct ASTCallFunction : public AST
{
    Token* name{nullptr};
    std::vector<std::unique_ptr<AST>> arguments;
};

} // namespace nusantara

#endif