/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_LEXER_TOKEN_TOKENS_H
#define NUSANTARA_LEXER_TOKEN_TOKENS_H

#include "nusantara/lexer/token/token.h"
#include "nusantara/support/input_stream.h"
#include <llvm/Support/raw_ostream.h>
#include <vector>

namespace nusantara {

struct Tokens
{
    InputStream* inputStream{nullptr};
    std::vector<Token> elements;
};

llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const Tokens& tokens);

} // namespace nusantara

#endif