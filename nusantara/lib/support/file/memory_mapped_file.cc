/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/support/file/memory_mapped_file.h"
#include <llvm/Support/raw_ostream.h>
#include <stdexcept>
#include <string>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <cstddef>
    #include <fcntl.h>
    #include <sys/mman.h>
    #include <sys/stat.h>
    #include <unistd.h>
#endif

namespace nusantara {

MemoryMappedFile::MemoryMappedFile(MemoryMappedFile&& other) noexcept : _chars(other._chars), _size(other._size)
{
    other._chars = nullptr;
    other._size = 0;

#ifdef _WIN32

    this->_mapping = other._mapping;
    this->_file = other._file;
    other._mapping = NULL;
    other._file = INVALID_HANDLE_VALUE;

#else

    this->_fd = other._fd;
    other._fd = -1;

#endif
}

MemoryMappedFile& MemoryMappedFile::operator=(MemoryMappedFile&& other) noexcept
{
    if (this != &other)
    {
        this->_reset();

        this->_chars = other._chars;
        this->_size = other._size;

#ifdef _WIN32

        this->_mapping = other._mapping;
        this->_file = other._file;
        other._mapping = NULL;
        other._file = INVALID_HANDLE_VALUE;

#else

        this->_fd = other._fd;
        other._fd = -1;

#endif

        other._chars = nullptr;
        other._size = 0;
    }
    return *this;
}

MemoryMappedFile::~MemoryMappedFile()
{
    this->_reset();
};

MemoryMappedFile MemoryMappedFile::create(const std::string& filePath)
{
    MemoryMappedFile mmf;

#ifdef _WIN32
    mmf._file = CreateFileA(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (mmf._file == INVALID_HANDLE_VALUE)
        throw std::runtime_error("Gagal membuka file.");

    DWORD fileSize = GetFileSize(mmf._file, NULL);
    if (fileSize == 0)
    {
        CloseHandle(mmf._file);
        mmf._file = INVALID_HANDLE_VALUE;
        return mmf;
    }

    mmf._mapping = CreateFileMapping(mmf._file, NULL, PAGE_READONLY, 0, 0, NULL);
    if (mmf._mapping == NULL)
    {
        CloseHandle(mmf._file);
        mmf._file = INVALID_HANDLE_VALUE;
        throw std::runtime_error("Gagal membuat pemetaan file.");
    }

    mmf._chars = static_cast<char*>(MapViewOfFile(mmf._mapping, FILE_MAP_READ, 0, 0, 0));
    if (mmf._chars == nullptr)
    {
        CloseHandle(mmf._mapping);
        mmf._mapping = NULL;
        CloseHandle(mmf._file);
        mmf._file = INVALID_HANDLE_VALUE;
        throw std::runtime_error("Gagal memetakan tampilan file.");
    }

    mmf._size = fileSize;

#else

    mmf._fd = open(filePath.c_str(), O_RDONLY);
    if (mmf._fd == -1)
        throw std::runtime_error("Gagal membuka file.");

    struct stat fileStat{};
    if (fstat(mmf._fd, &fileStat) == -1)
    {
        close(mmf._fd);
        throw std::runtime_error("Gagal mendapatkan ukuran file.");
    }

    size_t fileSize = fileStat.st_size;
    if (fileSize == 0)
    {
        close(mmf._fd);
        return mmf;
    }

    mmf._chars = static_cast<char*>(mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE, mmf._fd, 0));
    if (mmf._chars == MAP_FAILED)
    {
        mmf._chars = nullptr;
        close(mmf._fd);
        throw std::runtime_error("mmap gagal.");
    }

    madvise(mmf._chars, fileSize, MADV_SEQUENTIAL);

    mmf._size = fileSize;

#endif

    return mmf;
}

const char* MemoryMappedFile::chars() const
{
    return this->_chars;
}

const size_t& MemoryMappedFile::size() const
{
    return this->_size;
}

void MemoryMappedFile::_reset()
{
    if (this->_chars != nullptr)
    {
#ifdef _WIN32
        UnmapViewOfFile(this->_chars);
#else
        munmap(this->_chars, this->_size);
#endif

        this->_chars = nullptr;
        this->_size = 0;
    }

#ifdef _WIN32
    if (this->_mapping != NULL)
    {
        CloseHandle(this->_mapping);
        this->_mapping = NULL;
    }

    if (this->_file != INVALID_HANDLE_VALUE)
    {
        CloseHandle(this->_file);
        this->_file = INVALID_HANDLE_VALUE;
    }
#else
    if (this->_fd != -1)
    {
        close(this->_fd);
        this->_fd = -1;
    }
#endif
}

} // namespace nusantara