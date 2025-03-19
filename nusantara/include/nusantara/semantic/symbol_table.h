/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_SEMANTIC_SYMBOL_TABLE_H
#define NUSANTARA_SEMANTIC_SYMBOL_TABLE_H

#include "nusantara/lexer/token/token_type.h"
#include <string>
#include <utility>
#include <vector>

namespace nusantara {

struct SymbolTable
{
    std::vector<std::pair<std::string, std::vector<TokenType>>> functions;
    std::vector<std::pair<std::string, TokenType>> variables;
};

} // namespace nusantara

#endif