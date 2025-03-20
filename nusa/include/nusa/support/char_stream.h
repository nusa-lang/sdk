/*
 * ----------------------------------------------------------------------------
 * Project: Nusa
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, The Nusa Project Authors
 * ----------------------------------------------------------------------------
 */

#ifndef NUSA_SUPPORT_CHAR_STREAM_H
#define NUSA_SUPPORT_CHAR_STREAM_H

#include "nusa/support/memory_mapped_file.h"
#include <array>
#include <cstddef>
#include <map>
#include <optional>
#include <string_view>
#include <unordered_map>

namespace nusa {

class CharStream
{
public:
    CharStream();

    static CharStream cStr(const char* cstr);
    static CharStream file(const char* path);

    void setCStr(const char* cStr);
    void setFile(const char* path);

    const size_t& size();
    const size_t& index();
    const size_t& line();
    const size_t& column();

    void index(const size_t& index);

    bool end();
    void next();
    void next(const size_t& count);
    const char& charAt(const size_t& index);
    const char& cchar();

    bool match(const size_t& index, const char* chars);
    bool cmatch(const char* chars);

    void reset();
    void clear();

    const char* path() const;

    void saveStateTemp();
    void loadStateTemp();

    std::string_view lineView(const size_t& line);

private:
    std::optional<MemoryMappedFile> _file{std::nullopt};
    const char* _chars{nullptr};

    size_t _size{0};

    std::array<size_t, 3> _stateTemp{0, 0, 0};
    std::unordered_map<size_t, std::map<size_t, size_t>> _statesCache;

    size_t _index{0};
    size_t _line{0};
    size_t _column{0};

    void _set(const char* input, const bool& file);

    void _indexValidation(const size_t& index);
    void _lineValidation(const size_t& line);
};

}; // namespace nusa

#endif