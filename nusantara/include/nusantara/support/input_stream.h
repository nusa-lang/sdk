/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_SUPPORT_INPUT_STREAM_H
#define NUSANTARA_SUPPORT_INPUT_STREAM_H

#include "nusantara/support/memory_mapped_file.h"
#include <array>
#include <cstddef>
#include <map>
#include <optional>
#include <string_view>
#include <unordered_map>

namespace nusantara {

class InputStream
{
public:
    InputStream();

    InputStream(const char* input, const bool& file = false);

    void set(const char* input, const bool& file = false);

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

    const char* input() const;
    bool file();

    void saveStateTemp();
    void loadStateTemp();

    size_t maxIndex();
    size_t maxLine();
    std::string_view lineView(const size_t& line);

private:
    std::optional<MemoryMappedFile> _file{std::nullopt};
    const char* _input{nullptr};

    size_t _size{0};

    std::array<size_t, 3> _stateTemp{0, 0, 0};
    std::unordered_map<size_t, std::map<size_t, size_t>> _statesCache;

    size_t _index{0};
    size_t _line{0};
    size_t _column{0};

    void _indexValidation(const size_t& index);
    void _lineValidation(const size_t& line);
};

}; // namespace nusantara

#endif