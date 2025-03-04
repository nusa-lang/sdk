/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_SUPPORT_FILE_CHAR_STREAM_H
#define NUSANTARA_SUPPORT_FILE_CHAR_STREAM_H

#include "nusantara/support/char_stream/char_stream_base.h"
#include "nusantara/support/file/memory_mapped_file.h"
#include <cstddef>
#include <string>

namespace nusantara {

class FileCharStream : public CharStreamBase
{
public:
    explicit FileCharStream(std::string path);

    const char* peek(const size_t& index = 0) override;
    const char* previous() override;
    const char* current() override;
    const char* next() override;

    const std::string& path();

private:
    std::string _path;
    MemoryMappedFile _mmf;
};

} // namespace nusantara

#endif