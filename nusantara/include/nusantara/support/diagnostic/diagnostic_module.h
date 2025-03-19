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

#include "nusantara/support/out_stream.h"

namespace nusantara {

enum class DiagnosticModule
{
    Lexer,
    Parser
};

OutStream& operator<<(OutStream& os, const DiagnosticModule& diagnosticModule);

} // namespace nusantara

#endif