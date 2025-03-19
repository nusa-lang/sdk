/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_AST_ASTS_H
#define NUSANTARA_AST_ASTS_H

#include "nusantara/ast/ast.h"
#include "nusantara/support/char_stream.h"
#include <memory>
#include <vector>

namespace nusantara {

struct ASTS
{
    CharStream* charStream{nullptr};
    std::vector<std::unique_ptr<AST>> elements;
};

} // namespace nusantara

#endif