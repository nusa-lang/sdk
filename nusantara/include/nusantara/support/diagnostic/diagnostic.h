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

#include "nusantara/support/diagnostic/diagnostic_category.h"
#include "nusantara/support/diagnostic/diagnostic_module.h"
#include "nusantara/support/input_stream.h"
#include <cstddef>
#include <llvm/Support/raw_ostream.h>
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
    InputStream* inputStream;
    std::vector<Location> locations;
    std::string message;
};

llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const Diagnostic& diagnostic);

} // namespace nusantara

#endif