/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/support/diagnostic/diagnostic.h"
#include <llvm/Support/raw_ostream.h>
#include <string>

namespace nusantara {

llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const Diagnostic& diagnostic)
{
    if (diagnostic.inputStream)
    {
        os << (diagnostic.inputStream->file() ? diagnostic.inputStream->input() : "<unknown>");
        if (!diagnostic.locations.empty())
            os << ':';
    }
    else
        os << "<unknown>:";

    if (!diagnostic.locations.empty())
    {
        const auto& loc = diagnostic.locations.front();
        os << loc.line + 1 << ':' << loc.column + 1;
    }

    os << ' ' << diagnostic.module << ' ' << diagnostic.category << ": ";

    if (diagnostic.inputStream && !diagnostic.locations.empty())
    {
        os << "\n\n";
        for (const auto& loc : diagnostic.locations)
        {
            std::string prefix = std::to_string(loc.line + 1) + "| ";
            os << prefix << diagnostic.inputStream->lineView(loc.line) << '\n' << std::string(prefix.size() + loc.column, ' ') << std::string(loc.size, '^') << '\n';
        }
    }

    return os << diagnostic.message;
}

} // namespace nusantara