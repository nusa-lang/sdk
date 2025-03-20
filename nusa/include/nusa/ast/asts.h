/*
 * ----------------------------------------------------------------------------
 * Project: Nusa
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, The Nusa Project Authors
 * ----------------------------------------------------------------------------
 */

#ifndef NUSA_AST_ASTS_H
#define NUSA_AST_ASTS_H

#include "nusa/ast/ast.h"
#include "nusa/support/char_stream.h"
#include <memory>
#include <vector>

namespace nusa {

struct ASTS
{
    CharStream* charStream{nullptr};
    std::vector<std::unique_ptr<AST>> elements;
};

} // namespace nusa

#endif