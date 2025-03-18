/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_SUPPORT_DIAGNOSTIC_DIAGNOSTIC_H
#define NUSANTARA_SUPPORT_DIAGNOSTIC_DIAGNOSTIC_H

#include "nusantara/support/char_stream.h"
#include "nusantara/support/diagnostic/diagnostic_category.h"
#include "nusantara/support/diagnostic/diagnostic_module.h"
#include "nusantara/support/out_stream.h"
#include <cstddef>
#include <string>
#include <vector>

namespace nusantara {

struct Diagnostic
{
    struct Location
    {
        size_t line{0};
        size_t column{0};
        size_t size{0};
    };

    DiagnosticCategory category;
    DiagnosticModule module;
    CharStream* charStream;
    std::vector<Location> locations;
    std::string message;
};

OutStream& operator<<(OutStream& os, const Diagnostic& diagnostic);

} // namespace nusantara

#endif