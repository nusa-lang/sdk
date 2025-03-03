/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_SUPPORT_MEMORY_MAPPED_FILE_H
#define NUSANTARA_SUPPORT_MEMORY_MAPPED_FILE_H

#include <llvm/Support/raw_ostream.h>
#include <string>

namespace nusantara {

void memoryMappedFileRead(const std::string& filePath, llvm::raw_ostream& output);

}

#endif