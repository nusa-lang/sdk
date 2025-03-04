/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/support/char_stream/char_stream.h"
#include <cstddef>
#include <cstring>
#include <string>
#include <utility>

namespace nusantara {

CharStream::CharStream(std::string chars) : _chars(std::move(chars)) {}

const char* CharStream::peek(const size_t& index)
{
    if (index >= this->_chars.size())
        return nullptr;

    return &this->_chars[index];
}

const char* CharStream::previous()
{
    if (this->index() == 0)
        return nullptr;

    if (this->_chars[--this->_index] == '\n')
    {
        this->_line--;
        this->_column = 0;

        size_t tempIndex{this->_index - 1};
        const char* tempChar{this->peek(tempIndex)};
        while (tempChar != nullptr && *tempChar != '\n')
        {
            this->_column++;
            tempChar = this->peek(--tempIndex);
        }
    }
    else if (this->_column > 0)
        this->_column--;

    return &this->_chars[this->_index];
}

const char* CharStream::current()
{
    if (this->_index >= this->_chars.size())
        return nullptr;

    return &this->_chars[this->_index];
}

const char* CharStream::next()
{
    if (this->_index >= this->_chars.size())
        return nullptr;

    char c{this->_chars[this->_index++]};

    if (c == '\n')
    {
        this->_line++;
        this->_column = 0;
    }
    else
        this->_column++;

    return &this->_chars[this->_index];
}

} // namespace nusantara