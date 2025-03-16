/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/lexer/token/tokens.h"
#include <llvm/Support/Casting.h>
#include <llvm/Support/raw_ostream.h>
#include <string_view>

namespace nusantara {

llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const Tokens& tokens)
{
    std::string_view prefix = tokens.inputStream != nullptr && tokens.inputStream->file() ? tokens.inputStream->input() : std::string_view{};
    bool hasPrefix = !prefix.empty();

    auto it = tokens.elements.begin();
    auto end = tokens.elements.end();

    if (it != end)
    {
        if (auto* vecOS = llvm::dyn_cast<llvm::raw_svector_ostream>(&os))
            vecOS->reserveExtraSpace(tokens.elements.size() * 10);

        os << prefix;
        if (hasPrefix)
            os.write(":", 1);
        os << *it;

        while (++it != end)
        {
            os.write("\n", 1);
            os << prefix;
            if (hasPrefix)
                os.write(":", 1);
            os << *it;
        }
    }

    return os;
}

} // namespace nusantara