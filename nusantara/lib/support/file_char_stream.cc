/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/support/file_char_stream.h"
#include "nusantara/support/memory_mapped_file.h"
#include <llvm/Support/raw_ostream.h>
#include <string>

namespace nusantara {

FileCharStream::FileCharStream(const std::string& filePath)
{
    llvm::raw_string_ostream strs{this->rchars()};
    memoryMappedFileRead(filePath, strs);
}

} // namespace nusantara