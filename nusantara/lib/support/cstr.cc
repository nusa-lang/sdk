/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/support/cstr.h"
#include <cstddef>
#include <cstring>
#include <functional>
#include <string_view>

namespace nusantara {

size_t CStrHash::operator()(const char* str) const
{
    return std::hash<std::string_view>{}(str);
}

bool CStrEqual::operator()(const char* a, const char* b) const
{
    return std::strcmp(a, b) == 0;
}

} // namespace nusantara