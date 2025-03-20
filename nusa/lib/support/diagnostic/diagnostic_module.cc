/*
 * ----------------------------------------------------------------------------
 * Project: Nusa
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, The Nusa Project Authors
 * ----------------------------------------------------------------------------
 */

#include "nusa/support/diagnostic/diagnostic_module.h"
#include "nusa/support/out_stream.h"
#include <magic_enum.hpp>

namespace nusa {

OutStream& operator<<(OutStream& os, const DiagnosticModule& diagnosticModule)
{
    return os << magic_enum::enum_name(diagnosticModule);
}

} // namespace nusa