/*
 * ----------------------------------------------------------------------------
 * Project: Nusa
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, The Nusa Project Authors
 * ----------------------------------------------------------------------------
 */

#include "nusa/semantic/scopes.h"
#include "nusa/lexer/token/token_type.h"
#include <cstddef>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace nusa {

void Scopes::enterScope()
{
    this->_scopes.emplace_back();
}

void Scopes::exitScope()
{
    if (!this->_scopes.empty())
        this->_scopes.pop_back();
}

bool Scopes::empty()
{
    return this->_scopes.empty();
}

size_t Scopes::size()
{
    return this->_scopes.size();
}

void Scopes::createFunction(const char* name, const std::vector<TokenType>& parameters)
{
    if (this->_scopes.empty())
        throw std::runtime_error("No active scope.");

    if (this->callFunction(name, parameters))
        throw std::runtime_error("Redefining function.");

    this->_scopes.back().functions.emplace_back(name, parameters);
}

void Scopes::createVariable(const char* name, const TokenType& type)
{
    if (this->_scopes.empty())
        throw std::runtime_error("No active scope.");

    if (this->variable(name) != std::nullopt)
        throw std::runtime_error("Redefining variable.");

    this->_scopes.back().variables.emplace_back(name, type);
}

bool Scopes::callFunction(std::string_view name, const std::vector<TokenType>& arguments)
{
    for (auto it{this->_scopes.rbegin()}; it != this->_scopes.rend(); ++it)
        for (const auto& function : it->functions)
            if (function.first == name && function.second == arguments)
                return true;

    return false;
}

std::optional<TokenType> Scopes::variable(std::string_view name)
{
    for (auto it{this->_scopes.rbegin()}; it != this->_scopes.rend(); ++it)
        for (const auto& variable : it->variables)
            if (variable.first == name)
                return variable.second;

    return std::nullopt;
}

} // namespace nusa