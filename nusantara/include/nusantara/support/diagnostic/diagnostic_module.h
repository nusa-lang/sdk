/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_SUPPORT_DIAGNOSTIC_DIAGNOSTIC_MODULE_H
#define NUSANTARA_SUPPORT_DIAGNOSTIC_DIAGNOSTIC_MODULE_H

#include <llvm/Support/raw_ostream.h>

namespace nusantara {

enum class DiagnosticModule
{
    Lexer
};

llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const DiagnosticModule& diagnosticModule);

} // namespace nusantara

#endif