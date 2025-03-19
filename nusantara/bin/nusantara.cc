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
#include "nusantara/lexer/use_manager.h"
#include "nusantara/parser/parser.h"
#include "nusantara/support/diagnostic/diagnostics.h"
#include "nusantara/support/out_stream.h"
#include <cstdlib>
#include <exception>
#include <vector>

using namespace nusantara;

int main(int argc, char* argv[])
{
    try
    {
        outs() << "Nusantara Programming Language (Development)\n\n";

        UseManager mm;

        mm.push("examples/main.n");

        Lexer lexer;

        Diagnostics diagnostics;
        std::vector<Tokens> vecTokens{lexer.tokenization(mm, diagnostics)};

        if (!diagnostics.empty())
            outs() << diagnostics;

        if (diagnostics.hasError())
            return -1;

        // for (const auto& tokens : vecTokens)
        //     outs() << tokens << "\n";

        diagnostics.clear();

        Parser parser;
        parser.parse(vecTokens, diagnostics);

        if (!diagnostics.empty())
            outs() << diagnostics;

        if (diagnostics.hasError())
            return -1;

        outs() << "Program completed.\n";
    }
    catch (const std::exception& error)
    {
        outs() << error.what() << "\n";
        return 1;
    }

    return 0;
}