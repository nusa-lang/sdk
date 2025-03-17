/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/support/memory_mapped_file.h"
#include <cstddef>
#include <stdexcept>
#include <utility>

#ifdef _WIN32
    #include <fileapi.h>
    #include <handleapi.h>
    #include <memoryapi.h>
    #include <minwindef.h>
    #include <windows.h>
    #include <winnt.h>
#else
    #include <fcntl.h>
    #include <sys/mman.h>
    #include <sys/stat.h>
    #include <unistd.h>
#endif

namespace nusantara {

MemoryMappedFile::MemoryMappedFile() = default;

MemoryMappedFile::MemoryMappedFile(MemoryMappedFile&& other) noexcept
    : _path(std::exchange(other._path, nullptr)), _chars(std::exchange(other._chars, nullptr)), _size(std::exchange(other._size, 0))
#ifdef _WIN32
      ,
      _mapping(std::exchange(other._mapping, nullptr)), _file(std::exchange(other._file, nullptr))
#else
      ,
      _fd(std::exchange(other._fd, -1))
#endif
{
}

MemoryMappedFile& MemoryMappedFile::operator=(MemoryMappedFile&& other) noexcept
{
    if (this != &other)
    {
        this->clear();
        this->_path = std::exchange(other._path, nullptr);
        this->_chars = std::exchange(other._chars, nullptr);
        this->_size = std::exchange(other._size, 0);
#ifdef _WIN32
        this->_mapping = std::exchange(other._mapping, nullptr);
        this->_file = std::exchange(other._file, nullptr);
#else
        this->_fd = std::exchange(other._fd, -1);
#endif
    }
    return *this;
}

MemoryMappedFile::MemoryMappedFile(const char* path)
{
    this->set(path);
}

MemoryMappedFile::~MemoryMappedFile()
{
    this->clear();
}

void MemoryMappedFile::set(const char* path)
{
    this->clear();

    this->_path = path;

#ifdef _WIN32
    constexpr DWORD FILE_SHARE_ALL = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
    this->_file = CreateFileA(path, GENERIC_READ, FILE_SHARE_ALL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (this->_file == INVALID_HANDLE_VALUE)
        throw std::runtime_error("Gagal membuka berkas.");

    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(this->_file, &fileSize) || fileSize.QuadPart == 0)
        return;

    this->_mapping = CreateFileMapping(this->_file, NULL, PAGE_READONLY, 0, 0, NULL);
    if (!this->_mapping)
        throw std::runtime_error("Gagal membuat pemetaan berkas.");

    this->_chars = static_cast<char*>(MapViewOfFile(this->_mapping, FILE_MAP_READ, 0, 0, 0));
    if (!this->_chars)
        throw std::runtime_error("Gagal memetakan tampilan berkas.");

    this->_size = static_cast<size_t>(fileSize.QuadPart);
#else
    this->_fd = open(path, O_RDONLY);
    if (this->_fd == -1)
        throw std::runtime_error("Gagal membuka berkas.");

    struct stat fileStat{};
    if (fstat(this->_fd, &fileStat) == -1)
        throw std::runtime_error("Gagal mendapatkan ukuran berkas.");
    if ((this->_size = fileStat.st_size) == 0)
        return;

    this->_chars = static_cast<char*>(mmap(nullptr, this->_size, PROT_READ, MAP_PRIVATE | MAP_POPULATE, this->_fd, 0));
    if (this->_chars == MAP_FAILED)
        throw std::runtime_error("Gagal melakukan pemetaan memori.");

    if (this->_size > 64 * 1024) // If file is larger than 64KB, use sequential access hint
        madvise(this->_chars, this->_size, MADV_SEQUENTIAL);
    else
        madvise(this->_chars, this->_size, MADV_NORMAL);
#endif
}

const char* MemoryMappedFile::path() const
{
    return this->_path;
}

const char* MemoryMappedFile::chars() const
{
    return this->_chars;
}

const size_t& MemoryMappedFile::size() const
{
    return this->_size;
}

void MemoryMappedFile::clear()
{
#ifdef _WIN32
    if (this->_chars)
        UnmapViewOfFile(this->_chars);
    if (this->_mapping)
        CloseHandle(this->_mapping);
    if (this->_file != INVALID_HANDLE_VALUE)
        CloseHandle(this->_file);
#else
    if (this->_chars)
        munmap(this->_chars, _size);
    if (this->_fd != -1)
        close(this->_fd);
#endif
    this->_path = nullptr;
    this->_chars = nullptr;
    this->_size = 0;
}

}; // namespace nusantara