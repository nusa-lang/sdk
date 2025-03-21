/*
 * ----------------------------------------------------------------------------
 * Project: Nusa
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, The Nusa Project Authors
 * ----------------------------------------------------------------------------
 */

#include "nusa/parser/parser.h"
#include "nusa/ast/ast.h"
#include "nusa/ast/ast_call_function.h"
#include "nusa/ast/ast_function.h"
#include "nusa/ast/ast_literal.h"
#include "nusa/ast/ast_variable.h"
#include "nusa/ast/asts.h"
#include "nusa/lexer/token/token_type.h"
#include "nusa/lexer/token/tokens.h"
#include "nusa/support/char_stream.h"
#include "nusa/support/diagnostic/diagnostic.h"
#include "nusa/support/diagnostic/diagnostic_category.h"
#include "nusa/support/diagnostic/diagnostic_module.h"
#include "nusa/support/diagnostic/diagnostics.h"
#include <cstdint>
#include <exception>
#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace nusa {

std::unordered_set<TokenType> Parser::_dataTypes{
    TokenType::KW_DT_NUM,
    TokenType::KW_DT_STR,
};

Parser::Parser() = default;

std::vector<ASTS> Parser::parse(std::vector<Tokens>& vecTokens, Diagnostics& diagnostics)
{
    std::vector<ASTS> vecASTS;

    for (auto& tokens : vecTokens)
    {
        ASTS asts{this->parse(tokens, diagnostics)};
        if (!asts.elements.empty())
            vecASTS.emplace_back(std::move(asts));
    }

    return vecASTS;
}

ASTS Parser::parse(Tokens& tokens, Diagnostics& diagnostics)
{
    ASTS asts;
    asts.charStream = tokens.charStream;

    if (tokens.elements.empty())
        return asts;

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
                asts.elements.emplace_back(this->_parseFunction());
            else if (this->_is({TokenType::LIT_NUM, TokenType::LIT_STR, TokenType::ID}))
                asts.elements.emplace_back(this->_parsePrimaryExpression());
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

    return asts;
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

std::unique_ptr<AST> Parser::_parseFunction()
{
    auto astFunction{std::make_unique<ASTFunction>()};

    this->_match(TokenType::KW_F, "Expected 'f' keyword.");
    astFunction->name = this->_token;
    this->_match(TokenType::ID, "Expected function name.");
    this->_match(TokenType::PAREN_OPEN, "Expected '(' after function name.");

    // Parameter 1
    if (this->_is(_dataTypes))
    {
        ASTVariable astVariable;
        astVariable.type = this->_token;
        this->_match(_dataTypes);

        astVariable.name = this->_token;
        this->_match(TokenType::ID, "Expected parameter name.");

        astFunction->parameters.emplace_back(std::move(astVariable));

        // Parameter++
        while (this->_is(TokenType::COMMA))
        {
            this->_match(TokenType::COMMA);

            ASTVariable astVariable;
            astVariable.type = this->_token;
            this->_match(_dataTypes, "Expected parameter data type.");

            astVariable.name = this->_token;
            this->_match(TokenType::ID, "Expected parameter name.");

            astFunction->parameters.emplace_back(std::move(astVariable));
        }
    }

    this->_match(TokenType::PAREN_CLOSE, "Expected ')' after parameters.");

    return astFunction;
}

std::unique_ptr<AST> Parser::_parsePrimaryExpression()
{
    if (this->_is({TokenType::LIT_NUM, TokenType::LIT_STR}))
    {
        auto astLiteral{std::make_unique<ASTLiteral>()};

        astLiteral->token = this->_token;
        const auto& lexeme{this->_token->lexeme};

        if (this->_is(TokenType::LIT_STR))
            astLiteral->value = lexeme.substr(1, lexeme.size() - 2);
        else if (this->_is(TokenType::LIT_NUM))
        {
            const auto& size{lexeme.size()};

            if (lexeme.find(".") != std::string::npos)
            {
                double dValue{std::stod(lexeme)};

                if (dValue >= std::numeric_limits<float>::lowest() && dValue <= std::numeric_limits<float>::max())
                    astLiteral->value = static_cast<float>(dValue);
                else
                    astLiteral->value = dValue;
            }
            else
            {
                try
                {
                    long long num{std::stoll(lexeme)};

                    if (num >= std::numeric_limits<int8_t>::lowest() && num <= std::numeric_limits<int8_t>::max())
                        astLiteral->value = static_cast<int8_t>(num);
                    else if (num >= std::numeric_limits<int16_t>::lowest() && num <= std::numeric_limits<int16_t>::max())
                        astLiteral->value = static_cast<int16_t>(num);
                    else if (num >= std::numeric_limits<int32_t>::lowest() && num <= std::numeric_limits<int32_t>::max())
                        astLiteral->value = static_cast<int32_t>(num);
                    else if (num >= std::numeric_limits<int64_t>::lowest() && num <= std::numeric_limits<int64_t>::max())
                        astLiteral->value = static_cast<int64_t>(num);
                }
                catch (const std::exception& error)
                {
                    throw this->_diagnosticError("Num is too large or small.");
                }
            }
        }

        this->_match({TokenType::LIT_NUM, TokenType::LIT_STR});

        return astLiteral;
    }

    if (this->_is(TokenType::ID))
    {
        auto astCallFunction{std::make_unique<ASTCallFunction>()};

        astCallFunction->name = this->_token;
        this->_match(TokenType::ID, "Expected function name.");
        this->_match(TokenType::PAREN_OPEN, "Expected '(' after function name.");

        // Argument 1
        if (this->_is({TokenType::LIT_NUM, TokenType::LIT_STR, TokenType::ID}))
        {
            astCallFunction->arguments.emplace_back(this->_parsePrimaryExpression());

            // Argument++
            while (this->_is(TokenType::COMMA))
            {
                this->_match(TokenType::COMMA);
                astCallFunction->arguments.emplace_back(this->_parsePrimaryExpression());
            }
        }

        this->_match(TokenType::PAREN_CLOSE, "Expected ')' after arguments.");

        return astCallFunction;
    }

    throw this->_diagnosticError("Unexpected expression.");
}

} // namespace nusa