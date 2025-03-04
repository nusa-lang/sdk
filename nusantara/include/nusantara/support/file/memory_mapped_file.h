/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#if defined(_WIN32) || defined(__linux__) || defined(__APPLE__)

    #ifndef NUSANTARA_SUPPORT_MEMORY_MAPPED_FILE_H
        #define NUSANTARA_SUPPORT_MEMORY_MAPPED_FILE_H

        #include <cstddef>
        #include <llvm/Support/raw_ostream.h>
        #include <string>

        #ifdef _WIN32
            #include <windows.h>
        #endif

namespace nusantara {

class MemoryMappedFile
{
public:
    MemoryMappedFile(const MemoryMappedFile&) = delete;
    MemoryMappedFile& operator=(const MemoryMappedFile&) = delete;

    MemoryMappedFile(MemoryMappedFile&& other) noexcept;
    MemoryMappedFile& operator=(MemoryMappedFile&& other) noexcept;

    ~MemoryMappedFile();

    static MemoryMappedFile create(const std::string& filePath);

    [[nodiscard]] const char* chars() const;
    [[nodiscard]] const size_t& size() const;

private:
    MemoryMappedFile() = default;

    char* _chars{nullptr};
    size_t _size{0};

        #ifdef _WIN32
    HANDLE _mapping{NULL};
    HANDLE _file{INVALID_HANDLE_VALUE};
        #elif defined(__linux__) || defined(__APPLE__)
    int _fd{-1};
        #endif

    void _reset();
};

} // namespace nusantara

    #endif

#endif