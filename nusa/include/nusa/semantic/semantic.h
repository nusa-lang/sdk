/*
 * ----------------------------------------------------------------------------
 * Project: Nusa
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, The Nusa Project Authors
 * ----------------------------------------------------------------------------
 */

#ifndef NUSA_SEMANTIC_SEMANTIC_H
#define NUSA_SEMANTIC_SEMANTIC_H

#include "nusa/ast/ast.h"
#include "nusa/ast/ast_call_function.h"
#include "nusa/ast/ast_function.h"
#include "nusa/ast/asts.h"
#include "nusa/lexer/token/token.h"
#include "nusa/semantic/scopes.h"
#include "nusa/support/char_stream.h"
#include "nusa/support/diagnostic/diagnostics.h"
#include <memory>
#include <string>
#include <vector>

namespace nusa {

class Semantic
{
public:
    Semantic();

    std::vector<std::unique_ptr<AST>> analyze(std::vector<ASTS>& vecASTS, Diagnostics& diagnostics);
    std::vector<std::unique_ptr<AST>> analyze(ASTS& asts, Diagnostics& diagnostics);

private:
    bool _isLastAST{true};

    CharStream* _charStream{nullptr};
    Diagnostics* _diagnostics{nullptr};

    Scopes _scopes;

    void _diagnosticError(Token* token, std::string errMsg);
    void _diagnosticError(ASTFunction& function, std::string errMsg);
    void _diagnosticError(ASTCallFunction& callFunction, std::string errMsg);

    void _analyzeFunction(ASTFunction& function);
    void _analyzeCallFunction(ASTCallFunction& callFunction);
};

} // namespace nusa

#endif