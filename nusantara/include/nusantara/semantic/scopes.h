/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_SEMANTIC_SCOPES_H
#define NUSANTARA_SEMANTIC_SCOPES_H

#include "nusantara/lexer/token/token_type.h"
#include "nusantara/semantic/symbol_table.h"
#include <cstddef>
#include <optional>
#include <string_view>
#include <vector>

namespace nusantara {

class Scopes
{
public:
    void enterScope();
    void exitScope();

    bool empty();
    size_t size();

    void createFunction(const char* name, const std::vector<TokenType>& parameters);
    void createVariable(const char* name, const TokenType& type);
    bool callFunction(std::string_view name, const std::vector<TokenType>& arguments);
    std::optional<TokenType> variable(std::string_view name);

private:
    std::vector<SymbolTable> _scopes;
};

} // namespace nusantara

#endif