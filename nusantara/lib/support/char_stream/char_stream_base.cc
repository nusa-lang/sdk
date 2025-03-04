/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/support/char_stream/char_stream_base.h"
#include <cstddef>

namespace nusantara {

void CharStreamBase::reset()
{
    this->_index = 0;
    this->_line = 0;
    this->_column = 0;
}

const size_t& CharStreamBase::index() const noexcept
{
    return this->_index;
}

const size_t& CharStreamBase::line() const noexcept
{
    return this->_line;
}

const size_t& CharStreamBase::column() const noexcept
{
    return this->_column;
}

} // namespace nusantara