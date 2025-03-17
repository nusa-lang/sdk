/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/support/diagnostic/diagnostic_module.h"
#include <llvm/Support/raw_ostream.h>
#include <magic_enum.hpp>

namespace nusantara {

llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const DiagnosticModule& diagnosticModule)
{
    return os << magic_enum::enum_name(diagnosticModule);
}

} // namespace nusantara