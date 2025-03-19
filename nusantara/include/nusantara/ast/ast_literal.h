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

namespace nusantara {

struct ASTLiteral : public AST
{
    Token* value{nullptr};
};

} // namespace nusantara

#endif