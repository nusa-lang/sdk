/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/lexer/lexer.h"
#include "nusantara/lexer/token/tokens.h"
#include "nusantara/module/module_manager.h"
#include <cstdlib>
#include <exception>
#include <llvm/Support/raw_ostream.h>
#include <string>
#include <vector>

using namespace nusantara;

int main(int argc, char* argv[])
{
    try
    {
        llvm::outs() << "Nusantara Programming Language (Development)\n\n";
        ModuleManager mm;

        mm.push("examples/main.n");

        Lexer lexer;

        std::vector<Tokens> vectokens{lexer.tokenization(mm)};

        for (const auto& tokens : vectokens)
            llvm::outs() << tokens << "\n";

        llvm::outs() << "\nProgram completed.\n";
    }
    catch (const std::exception& error)
    {
        llvm::outs() << error.what() << "\n";
        return 1;
    }

    return 0;
}