/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_SUPPORT_DIAGNOSTIC_DIAGNOSTICS_H
#define NUSANTARA_SUPPORT_DIAGNOSTIC_DIAGNOSTICS_H

#include "nusantara/support/diagnostic/diagnostic.h"
#include <cstddef>
#include <llvm/Support/raw_ostream.h>
#include <vector>

namespace nusantara {

class Diagnostics
{
public:
    friend llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const Diagnostics& diagnostics);

    void add(Diagnostic diagnostic);

    bool hasError();

private:
    std::vector<Diagnostic> _children;
    size_t _error{0};
    size_t _warning{0};
};

} // namespace nusantara

#endif