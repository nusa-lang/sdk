/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/support/memory_mapped_file.h"
#include <llvm/Support/raw_ostream.h>
#include <stdexcept>
#include <string>

#ifdef _WIN32
    #include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)
    #include <cstddef>
    #include <fcntl.h>
    #include <sys/mman.h>
    #include <sys/stat.h>
    #include <unistd.h>
#endif

namespace nusantara {

void memoryMappedFileRead(const std::string& filePath, llvm::raw_ostream& output)
{
#ifdef _WIN32
    HANDLE file = CreateFileA(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE)
        throw std::runtime_error("Gagal membuka file.");

    HANDLE mapping = CreateFileMapping(file, nullptr, PAGE_READONLY, 0, 0, nullptr);
    if (!mapping)
    {
        CloseHandle(file);
        throw std::runtime_error("Gagal membuat pemetaan file.");
    }

    char* data = static_cast<char*>(MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0));
    if (!data)
    {
        CloseHandle(mapping);
        CloseHandle(file);
        throw std::runtime_error("Gagal memetakan tampilan file.");
    }

    DWORD fileSize = GetFileSize(file, nullptr);

    output.write(data, fileSize);

    UnmapViewOfFile(data);
    CloseHandle(mapping);
    CloseHandle(file);
#elif defined(__linux__) || defined(__APPLE__)
    int fd = open(filePath.c_str(), O_RDONLY);
    if (fd == -1)
        throw std::runtime_error("Gagal membuka file.");

    struct stat fileStat{};
    if (fstat(fd, &fileStat) == -1)
    {
        close(fd);
        throw std::runtime_error("Gagal mendapatkan ukuran file.");
    }

    size_t fileSize = fileStat.st_size;
    if (fileSize == 0)
    {
        close(fd);
        return;
    }

    char* data = static_cast<char*>(mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE, fd, 0));
    if (data == MAP_FAILED)
    {
        close(fd);
        throw std::runtime_error("mmap gagal.");
    }

    madvise(data, fileSize, MADV_SEQUENTIAL);

    output.write(data, fileSize);

    munmap(data, fileSize);
    close(fd);
#else
    throw std::runtime_error("OS tidak didukung.");
#endif
}

} // namespace nusantara