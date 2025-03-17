/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/support/input_stream.h"
#include "nusantara/support/file/memory_mapped_file.h"
#include <cstddef>
#include <cstring>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace nusantara {

InputStream::InputStream() = default;

InputStream::InputStream(const char* input, const bool& file)
{
    this->set(input, file);
}

void InputStream::set(const char* input, const bool& file)
{
    this->clear();

    if (file)
    {
        this->_file = MemoryMappedFile{input};
        this->_size = this->_file->size();
        this->_input = this->_file->chars();
    }
    else
    {
        this->_size = std::strlen(input);
        this->_input = input;
    }
}

const size_t& InputStream::size()
{
    return this->_size;
}

const size_t& InputStream::index()
{
    return this->_index;
}

const size_t& InputStream::line()
{
    return this->_line;
}

const size_t& InputStream::column()
{
    return this->_column;
}

void InputStream::index(const size_t& index)
{
    this->_indexValidation(index);
    this->_index = index;
}

bool InputStream::end()
{
    return ((this->maxIndex() > 0) ? (this->_index > this->maxIndex()) : true);
}

void InputStream::next()
{
    if (this->end())
        throw std::out_of_range("Cannot move to the next character: index " + std::to_string(this->_index) + " exceeds max (" + std::to_string(this->maxIndex()) + ").");

    if (!this->_statesCache.contains(this->_line))
        this->_statesCache[this->_line] = {};

    if (!this->_statesCache[this->_line].contains(this->_index))
        this->_statesCache[this->_line][this->_index] = this->_column;

    if (this->cchar() == '\n')
    {
        this->_line++;
        this->_column = 0;
    }
    else
        this->_column++;

    this->_index++;
}

void InputStream::next(const size_t& count)
{
    for (size_t i{0}; i < count; i++)
        this->next();
}

const char& InputStream::charAt(const size_t& index)
{
    this->_indexValidation(index);

    return this->_input[index];
}

const char& InputStream::cchar()
{
    return this->charAt(this->_index);
}

bool InputStream::match(const size_t& index, const char* chars)
{
    this->_indexValidation(index);

    size_t size = std::strlen(chars);

    if (size == 0)
        return false;

    size_t futureIndex = index + size - 1;
    if (futureIndex >= this->_size)
        return false;

    for (size_t i = 0; i < size; i++)
        if (this->_input[index + i] != chars[i])
            return false;

    return true;
}

bool InputStream::cmatch(const char* chars)
{
    return this->match(this->_index, chars);
}

void InputStream::reset()
{
    this->_index = 0;
    this->_line = 0;
    this->_column = 0;
}

void InputStream::clear()
{
    this->reset();
    this->_file = std::nullopt;
    this->_input = nullptr;
}

const char* InputStream::input() const
{
    if (this->_file.has_value())
        return this->_file.value().path();
    else
        return this->_input;
}

bool InputStream::file()
{
    return this->_file.has_value();
}

void InputStream::saveStateTemp()
{
    this->_stateTemp = {this->_index, this->_line, this->_column};
}

void InputStream::loadStateTemp()
{
    if (this->_stateTemp.empty())
        throw std::runtime_error("Cannot load empty _stateTemp.");

    this->_index = std::exchange(this->_stateTemp[0], 0);
    this->_line = std::exchange(this->_stateTemp[1], 0);
    this->_column = std::exchange(this->_stateTemp[2], 0);
}

size_t InputStream::maxIndex()
{
    return this->_size > 0 ? this->_size - 1 : 0;
}

size_t InputStream::maxLine()
{
    return this->_statesCache.size() > 0 ? this->_statesCache.size() - 1 : 0;
}

std::string_view InputStream::lineView(const size_t& line)
{
    this->_lineValidation(line);

    const auto& it{this->_statesCache.find(line)};
    if (it->second.size() > 1)
        return {this->_input + it->second.begin()->first, this->_input + (it->second.rbegin()->first + 1 == this->_size ? it->second.rbegin()->first + 1 : it->second.rbegin()->first)};
    else
        return {this->_input + it->second.begin()->first};
}

void InputStream::_indexValidation(const size_t& index)
{
    const auto& maxIndex{this->maxIndex()};
    if (index <= maxIndex)
        return;

    throw std::out_of_range("Index " + std::to_string(index) + " exceeds max (" + std::to_string(maxIndex) + ").");
}

void InputStream::_lineValidation(const size_t& line)
{
    const auto& maxLine{this->maxLine()};
    if (line <= maxLine)
        return;

    throw std::out_of_range("Line " + std::to_string(line) + " exceeds max (" + std::to_string(maxLine) + ").");
}

} // namespace nusantara