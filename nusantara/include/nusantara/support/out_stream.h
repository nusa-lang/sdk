/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_SUPPORT_OUT_STREAM_H
#define NUSANTARA_SUPPORT_OUT_STREAM_H

#include <cstddef>
#include <string>
#include <string_view>
#include <type_traits>

namespace nusantara {

class OutStream
{
public:
    OutStream& write(const char* buf, size_t size);

    OutStream& operator<<(const char& value);
    OutStream& operator<<(const char* value);
    OutStream& operator<<(const std::string& value);
    OutStream& operator<<(const std::string_view& value);

    template <typename T, typename std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
    OutStream& operator<<(const T& value)
    {
        *this << std::to_string(value);
        return *this;
    }
};

OutStream& outs();

} // namespace nusantara

#endif