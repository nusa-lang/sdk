/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_SUPPORT_CSTR_H
#define NUSANTARA_SUPPORT_CSTR_H

#include <cstddef>

namespace nusantara {

struct CStrHash
{
    size_t operator()(const char* str) const;
};

struct CStrEqual
{
    bool operator()(const char* a, const char* b) const;
};

} // namespace nusantara

#endif