/*
 * ----------------------------------------------------------------------------
 * Project: Nusa
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, The Nusa Project Authors
 * ----------------------------------------------------------------------------
 */

#ifndef NUSA_SEMANTIC_SYMBOL_TABLE_H
#define NUSA_SEMANTIC_SYMBOL_TABLE_H

#include "nusa/lexer/token/token_type.h"
#include <string>
#include <utility>
#include <vector>

namespace nusa {

struct SymbolTable
{
    std::vector<std::pair<std::string, std::vector<TokenType>>> functions;
    std::vector<std::pair<std::string, TokenType>> variables;
};

} // namespace nusa

#endif