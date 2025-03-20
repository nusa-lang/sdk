/*
 * ----------------------------------------------------------------------------
 * Project: Nusa
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, The Nusa Project Authors
 * ----------------------------------------------------------------------------
 */

#ifndef NUSA_SUPPORT_CSTR_H
#define NUSA_SUPPORT_CSTR_H

#include <cstddef>

namespace nusa {

struct CStrHash
{
    size_t operator()(const char* str) const;
};

struct CStrEqual
{
    bool operator()(const char* a, const char* b) const;
};

} // namespace nusa

#endif