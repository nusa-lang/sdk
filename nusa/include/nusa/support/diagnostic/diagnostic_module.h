/*
 * ----------------------------------------------------------------------------
 * Project: Nusa
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, The Nusa Project Authors
 * ----------------------------------------------------------------------------
 */

#ifndef NUSA_SUPPORT_DIAGNOSTIC_DIAGNOSTIC_MODULE_H
#define NUSA_SUPPORT_DIAGNOSTIC_DIAGNOSTIC_MODULE_H

#include "nusa/support/out_stream.h"

namespace nusa {

enum class DiagnosticModule
{
    Lexer,
    Parser,
    Semantic
};

OutStream& operator<<(OutStream& os, const DiagnosticModule& diagnosticModule);

} // namespace nusa

#endif