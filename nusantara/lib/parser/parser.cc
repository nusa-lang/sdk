/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/parser/parser.h"
#include "nusantara/lexer/token/token_type.h"
#include "nusantara/lexer/token/tokens.h"
#include "nusantara/support/char_stream.h"
#include "nusantara/support/diagnostic/diagnostic.h"
#include "nusantara/support/diagnostic/diagnostic_category.h"
#include "nusantara/support/diagnostic/diagnostic_module.h"
#include "nusantara/support/diagnostic/diagnostics.h"
#include <magic_enum.hpp>
#include <optional>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace nusantara {

std::unordered_set<TokenType> Parser::_dataTypes{
    TokenType::KW_DT_NUM,
    TokenType::KW_DT_STR,
};

Parser::Parser() = default;

void Parser::parse(std::vector<Tokens>& vecTokens, Diagnostics& diagnostics)
{
    for (auto& tokens : vecTokens)
        this->parse(tokens, diagnostics);
}

void Parser::parse(Tokens& tokens, Diagnostics& diagnostics)
{
    if (tokens.elements.empty())
        return;

    this->_diagnostics = &diagnostics;
    this->_tokens = &tokens;
    this->_token = &this->_tokens->elements[this->_index];

    bool hasError = false;
    bool hasPrevError = false;

    while (this->_notEof() && this->_canNextIndex())
    {
        try
        {
            if (this->_is(TokenType::KW_F))
                this->_parseFunction();
            else if (this->_is({TokenType::LIT_NUM, TokenType::LIT_STR, TokenType::ID}))
                this->_parseExpression();
            else
                throw this->_diagnosticError("Unexpected syntax.");

            hasPrevError = false;
        }
        catch (std::optional<Diagnostic>& diagnostic)
        {
            hasError = true;

            if (!hasPrevError && diagnostic)
                this->_diagnostics->add(std::move(diagnostic.value()));

            hasPrevError = true;

            this->_nextIndex();
        }
    }

    this->_diagnostics = nullptr;
    this->_tokens = nullptr;
    this->_token = nullptr;
    this->_index = 0;
}

bool Parser::_eof()
{
    return this->_is(TokenType::NEOF);
}

bool Parser::_notEof()
{
    return !this->_eof();
}

bool Parser::_canNextIndex()
{
    return this->_tokens != nullptr && this->_index < this->_tokens->elements.size();
}

void Parser::_nextIndex()
{
    this->_token = &this->_tokens->elements[++this->_index];
}

bool Parser::_is(const TokenType& type)
{
    return this->_token != nullptr && this->_token->type == type;
}

bool Parser::_is(const std::unordered_set<TokenType>& types)
{
    return this->_token != nullptr && types.contains(this->_token->type);
}

void Parser::_match(const TokenType& type, const std::optional<std::string>& errMsg)
{
    if (this->_is(type) && this->_canNextIndex())
        this->_nextIndex();
    else if (errMsg.has_value())
        throw this->_diagnosticError(std::move(errMsg.value()));
}

void Parser::_match(const std::unordered_set<TokenType>& types, const std::optional<std::string>& errMsg)
{
    if (this->_is(types) && this->_canNextIndex())
        this->_nextIndex();
    else if (errMsg.has_value())
        throw this->_diagnosticError(std::move(errMsg.value()));
}

std::optional<Diagnostic> Parser::_diagnosticError(std::string message)
{
    if (this->_diagnostics == nullptr)
        return std::nullopt;

    CharStream* charStream{nullptr};
    if (this->_tokens != nullptr)
        charStream = this->_tokens->charStream;

    std::vector<Diagnostic::Location> location;
    if (this->_token != nullptr)
        location.emplace_back(this->_token->line, this->_token->column, this->_token->lexeme.size());

    return Diagnostic{DiagnosticCategory::error, DiagnosticModule::Parser, charStream, std::move(location), std::move(message)};
}

void Parser::_parseFunction()
{
    this->_match(TokenType::KW_F, "Expected 'f' keyword.");
    this->_match(TokenType::ID, "Expected function name.");
    this->_match(TokenType::PAREN_OPEN, "Expected '(' after function name.");

    // Parameter 1
    if (this->_is(_dataTypes))
    {
        this->_match(_dataTypes);
        this->_match(TokenType::ID, "Expected parameter name.");

        // Parameter++
        while (this->_is(TokenType::COMMA))
        {
            this->_match(TokenType::COMMA);
            this->_match(_dataTypes, "Expected parameter data type.");
            this->_match(TokenType::ID, "Expected parameter name.");
        }
    }

    this->_match(TokenType::PAREN_CLOSE, "Expected ')' after parameters.");
}

void Parser::_parseExpression()
{
    if (this->_is(TokenType::LIT_NUM))
        this->_match(TokenType::LIT_NUM);
    else if (this->_is(TokenType::LIT_STR))
        this->_match(TokenType::LIT_STR);
    else if (this->_is(TokenType::ID))
    {
        this->_match(TokenType::ID, "Expected function name.");
        this->_match(TokenType::PAREN_OPEN, "Expected '(' after function name.");

        // Argument 1
        if (this->_is({TokenType::LIT_NUM, TokenType::LIT_STR, TokenType::ID}))
        {
            this->_parseExpression();

            // Argument++
            while (this->_is(TokenType::COMMA))
            {
                this->_match(TokenType::COMMA);
                this->_parseExpression();
            }
        }

        this->_match(TokenType::PAREN_CLOSE, "Expected ')' after arguments.");
    }
    else
        throw this->_diagnosticError("Unexpected expression.");
}

} // namespace nusantara