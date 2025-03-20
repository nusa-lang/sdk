/*
 * ----------------------------------------------------------------------------
 * Project: Nusa
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, The Nusa Project Authors
 * ----------------------------------------------------------------------------
 */

#ifndef NUSA_SUPPORT_DIAGNOSTIC_DIAGNOSTIC_H
#define NUSA_SUPPORT_DIAGNOSTIC_DIAGNOSTIC_H

#include "nusa/support/char_stream.h"
#include "nusa/support/diagnostic/diagnostic_category.h"
#include "nusa/support/diagnostic/diagnostic_module.h"
#include "nusa/support/out_stream.h"
#include <cstddef>
#include <string>
#include <vector>

namespace nusa {

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

} // namespace nusa

#endif