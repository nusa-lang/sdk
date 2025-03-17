/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/support/diagnostic/diagnostic_module.h"
#include "nusantara/support/out_stream.h"
#include <magic_enum.hpp>

namespace nusantara {

OutStream& operator<<(OutStream& os, const DiagnosticModule& diagnosticModule)
{
    return os << magic_enum::enum_name(diagnosticModule);
}

} // namespace nusantara