/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_SUPPORT_FILE_MEMORY_MAPPED_FILE_H
#define NUSANTARA_SUPPORT_FILE_MEMORY_MAPPED_FILE_H

#include <cstddef>

#ifdef _WIN32
    #include <windows.h>
    #include <winnt.h>
#else
    #include <fcntl.h>
    #include <sys/mman.h>
    #include <sys/stat.h>
    #include <unistd.h>
#endif

namespace nusantara {

class MemoryMappedFile
{
public:
    MemoryMappedFile();

    MemoryMappedFile(const MemoryMappedFile&) = delete;
    MemoryMappedFile& operator=(const MemoryMappedFile&) = delete;

    MemoryMappedFile(MemoryMappedFile&& other) noexcept;
    MemoryMappedFile& operator=(MemoryMappedFile&& other) noexcept;

    MemoryMappedFile(const char* path);

    ~MemoryMappedFile();

    void set(const char* path);

    [[nodiscard]] const char* path() const;
    [[nodiscard]] const char* chars() const;
    [[nodiscard]] const size_t& size() const;

    void clear();

private:
    const char* _path{nullptr};
    char* _chars{nullptr};
    size_t _size{0};

#ifdef _WIN32
    HANDLE _mapping{nullptr};
    HANDLE _file{nullptr};
#else
    int _fd{-1};
#endif
};

} // namespace nusantara

#endif