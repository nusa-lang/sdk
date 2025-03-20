/*
 * ----------------------------------------------------------------------------
 * Project: Nusa
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, The Nusa Project Authors
 * ----------------------------------------------------------------------------
 */

#include "nusa/support/diagnostic/diagnostics.h"
#include "nusa/support/diagnostic/diagnostic.h"
#include "nusa/support/diagnostic/diagnostic_category.h"
#include "nusa/support/out_stream.h"
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <utility>

namespace nusa {

OutStream& operator<<(OutStream& os, const Diagnostics& diagnostics)
{
    for (size_t i{0}; i < diagnostics._children.size(); ++i)
    {
        os << diagnostics._children[i];
        if (i + 1 < diagnostics._children.size() && !diagnostics._children[i].locations.empty())
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
    if (child.category == diagnostic.category && child.charStream && diagnostic.charStream && ((child.charStream->path() && diagnostic.charStream->path() && child.charStream->path() == diagnostic.charStream->path()) || (!child.charStream->path() && !diagnostic.charStream->path())) && child.module == diagnostic.module && child.message == diagnostic.message)
    {
        for (auto& location : diagnostic.locations)
        {
            if (child.locations.empty())
            {
                child.locations.emplace_back(std::move(location));
                continue;
            }

            auto& childEndLoc = child.locations.back();
            int lineDiff = std::abs(static_cast<int>(childEndLoc.line - location.line));

            if (childEndLoc.line == location.line) // Same line
            {
                int newStart = std::min(childEndLoc.column, location.column);
                int newEnd = std::max(childEndLoc.column + childEndLoc.size, location.column + location.size);
                childEndLoc.column = newStart;
                childEndLoc.size = newEnd - newStart;
            }
            else if (lineDiff == 1) // Only 1 line difference
            {
                child.locations.emplace_back(std::move(location));
            }
            else // Spacing more than 1 line, not merged
            {
                _children.emplace_back(std::move(diagnostic));
                return;
            }
        }
        return;
    }

    _children.emplace_back(std::move(diagnostic));
}

bool Diagnostics::empty()
{
    return this->_children.empty();
}

bool Diagnostics::hasError()
{
    return this->_error > 0;
}

void Diagnostics::clear()
{
    this->_children.clear();
    this->_error = 0;
    this->_warning = 0;
}

} // namespace nusa