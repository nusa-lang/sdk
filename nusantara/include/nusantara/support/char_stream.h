/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_SUPPORT_CHAR_STREAM_H
#define NUSANTARA_SUPPORT_CHAR_STREAM_H

#include <cstddef>
#include <string>

namespace nusantara {

class CharStream
{
public:
    CharStream();
    CharStream(const CharStream&) = default;
    CharStream(CharStream&&) = delete;
    CharStream& operator=(const CharStream&) = default;
    CharStream& operator=(CharStream&&) = delete;
    virtual ~CharStream() = default;

    explicit CharStream(std::string chars);

    virtual const char* peek(const size_t& index = 0) noexcept;
    const char* previous();
    virtual const char* current() noexcept;
    virtual const char* next();

    void reset() noexcept;
    void clear();

    [[nodiscard]] const size_t& index() const noexcept;
    [[nodiscard]] const size_t& line() const noexcept;
    [[nodiscard]] const size_t& column() const noexcept;

private:
    std::string _chars;

    size_t _index{0};

    size_t _line{0};
    size_t _column{0};

protected:
    std::string& rchars() noexcept;
};

} // namespace nusantara

#endif