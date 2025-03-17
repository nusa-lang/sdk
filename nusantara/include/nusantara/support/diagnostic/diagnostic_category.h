/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_SUPPORT_DIAGNOSTIC_DIAGNOSTIC_CATEGORY_H
#define NUSANTARA_SUPPORT_DIAGNOSTIC_DIAGNOSTIC_CATEGORY_H

#include <llvm/Support/raw_ostream.h>

namespace nusantara {

enum class DiagnosticCategory
{
    warning,
    error
};

llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const DiagnosticCategory& diagnosticCategory);

} // namespace nusantara

#endif