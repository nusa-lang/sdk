/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_SUPPORT_CHAR_STREAM_BASE_H
#define NUSANTARA_SUPPORT_CHAR_STREAM_BASE_H

#include <cstddef>

namespace nusantara {

class CharStreamBase
{
public:
    CharStreamBase() = default;
    CharStreamBase(const CharStreamBase&) = default;
    CharStreamBase(CharStreamBase&&) = default;
    CharStreamBase& operator=(const CharStreamBase&) = default;
    CharStreamBase& operator=(CharStreamBase&&) = default;

    virtual ~CharStreamBase() = default;

    virtual const char* peek(const size_t& index = 0) = 0;
    virtual const char* previous() = 0;
    virtual const char* current() = 0;
    virtual const char* next() = 0;

    void reset();

    [[nodiscard]] const size_t& index() const noexcept;
    [[nodiscard]] const size_t& line() const noexcept;
    [[nodiscard]] const size_t& column() const noexcept;

protected:
    size_t _index{0};

    size_t _line{0};
    size_t _column{0};
};

} // namespace nusantara

#endif