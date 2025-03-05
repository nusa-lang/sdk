#include "nusantara/lexer/lexer.h"
#include "nusantara/lexer/token/token.h"
#include "nusantara/lexer/token/token_type.h"
#include <exception>
#include <llvm/Support/raw_ostream.h>

int main(int argc, char* argv[])
{
    try
    {
        if (argc == 1)
            return 0;

        auto lexer{nusantara::Lexer::file(argv[1])};

        nusantara::Token token{lexer.nextToken()};
        while (token.type != nusantara::TokenType::NEOF)
        {
            llvm::outs() << token << "\n";
            token = lexer.nextToken();
        }

        llvm::outs() << token << "\n";

        return 0;
    }
    catch (const std::exception& error)
    {
        llvm::outs() << error.what() << "\n";
    }

    return 1;
}