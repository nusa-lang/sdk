/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_SEMANTIC_SEMANTIC_H
#define NUSANTARA_SEMANTIC_SEMANTIC_H

#include "nusantara/ast/ast.h"
#include "nusantara/ast/ast_call_function.h"
#include "nusantara/ast/ast_function.h"
#include "nusantara/ast/asts.h"
#include "nusantara/lexer/token/token.h"
#include "nusantara/semantic/scopes.h"
#include "nusantara/support/char_stream.h"
#include "nusantara/support/diagnostic/diagnostics.h"
#include <memory>
#include <string>
#include <vector>

namespace nusantara {

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

} // namespace nusantara

#endif