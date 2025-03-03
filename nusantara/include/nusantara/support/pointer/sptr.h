/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_SUPPORT_POINTER_SPTR_H
#define NUSANTARA_SUPPORT_POINTER_SPTR_H

#include <cstddef>
#include <stdexcept>

namespace nusantara {

template <typename T>
class SPTR
{
public:
    // Copy constructor
    SPTR(const SPTR& other) : _ptr(other._ptr) {}

    // Move constructor
    SPTR(SPTR&& other) noexcept : _ptr(other._ptr)
    {
        other._ptr = nullptr;
    }

    // Copy assignment operator
    SPTR& operator=(const SPTR& other)
    {
        if (this != &other)
            this->_ptr = other._ptr;

        return *this;
    }

    // Move assignment operator
    SPTR& operator=(SPTR&& other) noexcept
    {
        if (this != &other)
        {
            this->_ptr = other._ptr;
            other._ptr = nullptr;
        }

        return *this;
    }

    // Assignment operator
    SPTR& operator=(std::nullptr_t)
    {
        this->_ptr = nullptr;
        return *this;
    }

    SPTR& operator=(T* ref)
    {
        this->_ptr = ref;
        return *this;
    }

    // Nullptr Constructor
    SPTR() = default;

    // Constructor
    explicit SPTR(T& ref) : _ptr(&ref) {}

    // Destructor
    ~SPTR() = default;

    // Dereference operator
    T& operator*()
    {
        if (this->_ptr == nullptr)
            throw std::runtime_error("Tidak dapat mengakses null pointer.");

        return *this->_ptr;
    }

    // Arrow operator
    T* operator->()
    {
        if (this->_ptr == nullptr)
            throw std::runtime_error("Tidak dapat mengakses null pointer.");

        return this->_ptr;
    }

    // == operator
    bool operator==(std::nullptr_t) const
    {
        return _ptr == nullptr;
    }

    // != operator
    bool operator!=(std::nullptr_t) const
    {
        return _ptr != nullptr;
    }

private:
    T* _ptr{nullptr};
};

template <typename T>
using sptr = SPTR<T>;

} // namespace nusantara

#endif