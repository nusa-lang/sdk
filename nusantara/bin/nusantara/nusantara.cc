/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/lexer/lexer.h"
#include <exception>
#include <llvm/Support/raw_ostream.h>

int main(int argc, char* argv[])
{
    try
    {
        if (argc == 1)
            return 0;

        auto lexer{nusantara::Lexer::file(argv[1])};

        for (const auto& tokens : lexer.getVecTokens())
            llvm::outs() << tokens << "\n";

        return 0;
    }
    catch (const std::exception& error)
    {
        llvm::outs() << error.what() << "\n";
    }

    return 1;
}