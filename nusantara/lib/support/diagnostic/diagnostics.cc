/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/support/diagnostic/diagnostics.h"
#include "nusantara/support/diagnostic/diagnostic.h"
#include "nusantara/support/diagnostic/diagnostic_category.h"
#include "nusantara/support/out_stream.h"
#include <algorithm>
#include <cstddef>
#include <utility>

namespace nusantara {

OutStream& operator<<(OutStream& os, const Diagnostics& diagnostics)
{
    for (size_t i{0}; i < diagnostics._children.size(); ++i)
    {
        os << diagnostics._children[i];
        if (i + 1 < diagnostics._children.size())
            os << "\n\n────────────────────────────────\n";
        else
            os << '\n';
    }
    return os;
}

void Diagnostics::add(Diagnostic diagnostic)
{
    switch (diagnostic.category)
    {
    case DiagnosticCategory::warning:
        this->_warning++;
        break;
    case DiagnosticCategory::error:
        this->_error++;
        break;
    }

    if (_children.empty())
    {
        _children.emplace_back(std::move(diagnostic));
        return;
    }

    auto& child = _children.back();
    if (child.category == diagnostic.category && child.inputStream && diagnostic.inputStream && child.inputStream->file() && diagnostic.inputStream->file() && child.inputStream->input() == diagnostic.inputStream->input() && child.module == diagnostic.module && child.message == diagnostic.message)
    {

        for (auto& location : diagnostic.locations)
        {
            if (child.locations.empty())
            {
                child.locations.emplace_back(std::move(location));
                continue;
            }

            auto& childEndLoc = child.locations.back();
            if (childEndLoc.line == location.line)
            {
                int newStart = std::min(childEndLoc.column, location.column);
                int newEnd = std::max(childEndLoc.column + childEndLoc.size, location.column + location.size);
                childEndLoc.column = newStart;
                childEndLoc.size = newEnd - newStart;
            }
            else
            {
                child.locations.emplace_back(std::move(location));
            }
        }
        return;
    }

    _children.emplace_back(std::move(diagnostic));
}

bool Diagnostics::hasError()
{
    return this->_error > 0;
}

} // namespace nusantara