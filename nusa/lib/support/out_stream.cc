/*
 * ----------------------------------------------------------------------------
 * Project: Nusa
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, The Nusa Project Authors
 * ----------------------------------------------------------------------------
 */

#include "nusa/support/out_stream.h"
#include <cstddef>
#include <string>
#include <string_view>

#ifdef _WIN32
    #include <windows.h>

    #include <consoleapi.h>
    #include <processenv.h>
#else
    #include <cstring>
    #include <unistd.h>
#endif

namespace nusa {

OutStream& OutStream::write(const char* buf, size_t size)
{
#ifdef _WIN32
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), buf, size, nullptr, nullptr);
#else
    ::write(1, buf, size);
#endif
    return *this;
}

OutStream& OutStream::operator<<(const char& value)
{
    this->write(&value, 1);
    return *this;
}

OutStream& OutStream::operator<<(const char* value)
{
    this->write(value, strlen(value));
    return *this;
}

OutStream& OutStream::operator<<(const std::string& value)
{
    this->write(value.c_str(), value.size());
    return *this;
}

OutStream& OutStream::operator<<(const std::string_view& value)
{
    this->write(value.data(), value.size());
    return *this;
}

OutStream& outs()
{
    static OutStream outs;
    return outs;
}

} // namespace nusa