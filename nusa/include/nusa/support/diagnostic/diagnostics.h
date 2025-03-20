/*
 * ----------------------------------------------------------------------------
 * Project: Nusa
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, The Nusa Project Authors
 * ----------------------------------------------------------------------------
 */

#ifndef NUSA_SUPPORT_DIAGNOSTIC_DIAGNOSTICS_H
#define NUSA_SUPPORT_DIAGNOSTIC_DIAGNOSTICS_H

#include "nusa/support/diagnostic/diagnostic.h"
#include "nusa/support/out_stream.h"
#include <cstddef>
#include <vector>

namespace nusa {

class Diagnostics
{
public:
    friend OutStream& operator<<(OutStream& os, const Diagnostics& diagnostics);

    void add(Diagnostic diagnostic);

    bool empty();
    bool hasError();

    void clear();

private:
    std::vector<Diagnostic> _children;
    size_t _error{0};
    size_t _warning{0};
};

} // namespace nusa

#endif