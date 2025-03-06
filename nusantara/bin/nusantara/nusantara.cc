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
#include <iostream>
#include <llvm/Support/raw_ostream.h>
#include <string>

int main(int argc, char* argv[])
{
    try
    {
        nusantara::Lexer lexer;

        if (argc == 1)
        {
            while (true)
            {
                std::string input;
                llvm::outs() << "unknown:0:0 ";
                std::getline(std::cin, input);

                if (input == "exit()")
                    break;

                try
                {
                    for (const auto& tokens : lexer.input(input))
                        llvm::outs() << tokens << "\n";
                }
                catch (const std::exception& error)
                {
                    llvm::outs() << error.what() << "\n";
                }
            }

            return 0;
        }

        for (const auto& tokens : lexer.file(argv[1]))
            llvm::outs() << tokens << "\n";

        return 0;
    }
    catch (const std::exception& error)
    {
        llvm::outs() << error.what() << "\n";
    }

    return 1;
}