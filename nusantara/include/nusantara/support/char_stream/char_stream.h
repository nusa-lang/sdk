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

#include "nusantara/support/char_stream/char_stream_base.h"
#include <cstddef>
#include <string>

namespace nusantara {

class CharStream : public CharStreamBase
{
public:
    explicit CharStream(std::string chars);

    const char* peek(const size_t& index = 0) override;
    const char* previous() override;
    const char* current() override;
    const char* next() override;

private:
    std::string _chars;
};

} // namespace nusantara

#endif