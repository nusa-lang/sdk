/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/semantic/semantic.h"
#include "nusantara/ast/ast.h"
#include "nusantara/ast/ast_call_function.h"
#include "nusantara/ast/ast_function.h"
#include "nusantara/ast/ast_literal.h"
#include "nusantara/ast/asts.h"
#include "nusantara/lexer/token/token.h"
#include "nusantara/lexer/token/token_type.h"
#include "nusantara/support/diagnostic/diagnostic.h"
#include "nusantara/support/diagnostic/diagnostic_category.h"
#include "nusantara/support/diagnostic/diagnostic_module.h"
#include "nusantara/support/diagnostic/diagnostics.h"
#include <exception>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace nusantara {

Semantic::Semantic() = default;

std::vector<std::unique_ptr<AST>> Semantic::analyze(std::vector<ASTS>& vecASTS, Diagnostics& diagnostics)
{
    std::vector<std::unique_ptr<AST>> vecAST;

    for (auto it = vecASTS.begin(); it != vecASTS.end(); ++it)
    {
        this->_isLastAST = (std::next(it) == vecASTS.end());

        auto analyzedAST = this->analyze(*it, diagnostics);
        vecAST.insert(vecAST.end(), std::make_move_iterator(analyzedAST.begin()), std::make_move_iterator(analyzedAST.end()));
    }

    return vecAST;
}

std::vector<std::unique_ptr<AST>> Semantic::analyze(ASTS& asts, Diagnostics& diagnostics)
{
    if (this->_scopes.empty())
        this->_scopes.enterScope();

    this->_charStream = asts.charStream;
    this->_diagnostics = &diagnostics;

    std::erase_if(asts.elements,
        [this](auto& ast)
        {
            if (!ast)
                return true; // Remove jika nullptr
            if (dynamic_cast<ASTLiteral*>(ast.get()))
                return true; // Remove ASTLiteral

            try
            {
                if (auto* astFunction = dynamic_cast<ASTFunction*>(ast.get()))
                    this->_analyzeFunction(*astFunction);
                else if (auto* astCallFunction = dynamic_cast<ASTCallFunction*>(ast.get()))
                    this->_analyzeCallFunction(*astCallFunction);
                else
                {
                    this->_diagnosticError(nullptr, "AST cannot be analyzed.");
                    return true;
                }
            }
            catch (const std::exception& error)
            {
                if (auto* astFunction = dynamic_cast<ASTFunction*>(ast.get()))
                    this->_diagnosticError(*astFunction, error.what());
                else if (auto* astCallFunction = dynamic_cast<ASTCallFunction*>(ast.get()))
                    this->_diagnosticError(*astCallFunction, error.what());
                else
                    this->_diagnosticError(nullptr, "Unknown AST error.");
                return true;
            }

            return false;
        });

    this->_charStream = nullptr;
    this->_diagnostics = nullptr;

    if (this->_scopes.size() == 1 && this->_isLastAST)
        this->_scopes.exitScope();

    return std::move(asts.elements);
}

void Semantic::_diagnosticError(Token* token, std::string errMsg)
{
    if (this->_diagnostics == nullptr)
        return;

    std::vector<Diagnostic::Location> locations;
    if (token != nullptr)
        locations.emplace_back(token->line, token->column, token->lexeme.size());

    this->_diagnostics->add({DiagnosticCategory::error, DiagnosticModule::Semantic, this->_charStream, std::move(locations), std::move(errMsg)});
}

void Semantic::_diagnosticError(ASTFunction& function, std::string errMsg)
{
    this->_diagnosticError(function.name, errMsg);
    for (auto& parameter : function.parameters)
    {
        this->_diagnosticError(parameter.name, errMsg);
        this->_diagnosticError(parameter.type, errMsg);
    }
}

void Semantic::_diagnosticError(ASTCallFunction& callFunction, std::string errMsg)
{
    this->_diagnosticError(callFunction.name, errMsg);

    for (auto& argument : callFunction.arguments)
    {
        if (auto* astLiteral{dynamic_cast<ASTLiteral*>(argument.get())})
            this->_diagnosticError(astLiteral->value, errMsg);
        else if (auto* astCallFunction{dynamic_cast<ASTCallFunction*>(argument.get())})
            this->_diagnosticError(*astCallFunction, errMsg);
        else
            this->_diagnosticError(callFunction.name, "Have arguments that cannot be analyzed.");
    }
}

void Semantic::_analyzeFunction(ASTFunction& function)
{
    if (function.name == nullptr)
    {
        this->_diagnosticError(nullptr, "Missing function name.");
        return;
    }

    std::vector<TokenType> parameters;
    for (auto& parameter : function.parameters)
    {
        if (parameter.type == nullptr)
            this->_diagnosticError(nullptr, "Missing parameter data type.");

        parameters.emplace_back(parameter.type->type);
    }

    this->_scopes.createFunction(function.name->lexeme.c_str(), parameters);
}

void Semantic::_analyzeCallFunction(ASTCallFunction& callFunction)
{
    if (callFunction.name == nullptr)
    {
        this->_diagnosticError(nullptr, "Missing function name.");
        return;
    }

    std::vector<TokenType> arguments;
    for (auto& argument : callFunction.arguments)
    {
        if (auto* astLiteral{dynamic_cast<ASTLiteral*>(argument.get())})
        {
            if (astLiteral->value == nullptr)
                this->_diagnosticError(nullptr, "Missing literal value.");

            if (astLiteral->value->type == TokenType::LIT_NUM)
                arguments.emplace_back(TokenType::KW_DT_NUM);
            else if (astLiteral->value->type == TokenType::LIT_STR)
                arguments.emplace_back(TokenType::KW_DT_STR);
            else
                this->_diagnosticError(astLiteral->value, "Literals cannot be analyzed for their data type.");
        }
        else if (auto* astCallFunction{dynamic_cast<ASTCallFunction*>(argument.get())})
        {
            this->_analyzeCallFunction(*astCallFunction);
            this->_diagnosticError(*astCallFunction, "Function does not return anything.");
        }
        else
            this->_diagnosticError(callFunction.name, "Have arguments that cannot be analyzed.");
    }

    if (!this->_scopes.callFunction(callFunction.name->lexeme.c_str(), arguments))
        throw std::runtime_error("Function with that parameter does not exist.");
}

} // namespace nusantara