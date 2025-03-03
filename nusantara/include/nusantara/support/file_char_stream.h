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

#include "nusantara/support/char_stream.h"
#include <string>

namespace nusantara {

class FileCharStream : public CharStream
{
public:
    explicit FileCharStream(const std::string& filePath);
};

} // namespace nusantara

#endif