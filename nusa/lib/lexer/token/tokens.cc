/*
 * ----------------------------------------------------------------------------
 * Project: Nusa
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, The Nusa Project Authors
 * ----------------------------------------------------------------------------
 */

#include "nusa/lexer/token/tokens.h"
#include "nusa/support/out_stream.h"
#include <string_view>

namespace nusa {

OutStream& operator<<(OutStream& os, const Tokens& tokens)
{
    std::string_view prefix = tokens.charStream != nullptr && tokens.charStream->path() != nullptr ? tokens.charStream->path() : std::string_view{};
    bool hasPrefix = !prefix.empty();

    auto it = tokens.elements.begin();
    auto end = tokens.elements.end();

    if (it != end)
    {
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

} // namespace nusa