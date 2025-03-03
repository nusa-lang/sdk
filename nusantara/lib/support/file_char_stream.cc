/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/support/file_char_stream.h"
#include "nusantara/support/char_stream.h"
#include <cstddef>
#include <stdexcept>
#include <string>

namespace nusantara {

FileCharStream::FileCharStream(const std::string& file) : file(file)
{
    if (!this->file.is_open())
        throw std::runtime_error("Tidak dapat membuka file '" + file + "'.");
}

const char* FileCharStream::peek(const size_t& index) noexcept
{
    while (this->rchars().size() <= index)
        if (!this->addCharToRchars())
            return nullptr;

    return CharStream::peek(index);
}

const char* FileCharStream::current() noexcept
{
    if (this->rchars().empty())
        this->addCharToRchars();

    return CharStream::current();
}

const char* FileCharStream::next() noexcept
{
    if (!this->addCharToRchars())
        return nullptr;

    return CharStream::next();
}

bool FileCharStream::addCharToRchars()
{
    char tchar{};

    if (this->file.get(tchar))
    {
        this->rchars() += tchar;
        return true;
    }

    return false;
}

} // namespace nusantara