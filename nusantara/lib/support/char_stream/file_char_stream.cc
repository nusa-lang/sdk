/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/support/char_stream/file_char_stream.h"
#include <cstddef>
#include <string>
#include <utility>

namespace nusantara {

FileCharStream::FileCharStream(std::string filePath) : _filePath(std::move(filePath)), _mmf{MemoryMappedFile::create(this->_filePath)} {}

const char* FileCharStream::peek(const size_t& index)
{
    if (index >= this->_mmf.size())
        return nullptr;

    return &this->_mmf.chars()[index];
}

const char* FileCharStream::previous()
{
    if (this->index() == 0)
        return nullptr;

    if (this->_mmf.chars()[--this->_index] == '\n')
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

    return &this->_mmf.chars()[this->_index];
}

const char* FileCharStream::current()
{
    if (this->_index >= this->_mmf.size())
        return nullptr;

    return &this->_mmf.chars()[this->_index];
}

const char* FileCharStream::next()
{
    if (this->_index >= this->_mmf.size())
        return nullptr;

    char c{this->_mmf.chars()[this->_index++]};

    if (c == '\n')
    {
        this->_line++;
        this->_column = 0;
    }
    else
        this->_column++;

    return &this->_mmf.chars()[this->_index];
}

} // namespace nusantara