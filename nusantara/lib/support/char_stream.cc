/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/support/char_stream.h"
#include <cstddef>
#include <cstring>
#include <string>
#include <utility>

namespace nusantara {

CharStream::CharStream() = default;

CharStream::CharStream(std::string chars) : _chars(std::move(chars)) {}

const char* CharStream::peek(const size_t& index) noexcept
{
    if (index >= this->_chars.size())
        return nullptr;

    return &this->_chars[index];
}

const char* CharStream::previous()
{
    if (this->_index == 0)
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

const char* CharStream::current() noexcept
{
    if (this->_index >= this->_chars.size())
        return nullptr;

    return &this->_chars[this->_index];
}

const char* CharStream::next()
{
    size_t size{this->_chars.size()};
    if (this->_index >= (size == 0 ? size : size - 1))
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

void CharStream::reset() noexcept
{
    this->_index = 0;
    this->_line = 0;
    this->_column = 0;
}

void CharStream::clear()
{
    this->reset();
    this->_chars.clear();
}

const size_t& CharStream::index() const noexcept
{
    return this->_index;
}

const size_t& CharStream::line() const noexcept
{
    return this->_line;
}

const size_t& CharStream::column() const noexcept
{
    return this->_column;
}

std::string& CharStream::rchars() noexcept
{
    return this->_chars;
}

} // namespace nusantara