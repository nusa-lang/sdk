/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_SUPPORT_POINTER_HPTR_H
#define NUSANTARA_SUPPORT_POINTER_HPTR_H

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <unordered_map>

namespace nusantara {

template <typename T>
class HPTR
{
public:
    // Copy constructor
    HPTR(const HPTR& other) : _ptr(other._ptr), _address(other._address)
    {
        this->_registerAddressMap();
    }

    // Move constructor
    HPTR(HPTR&& other) noexcept : _ptr(other._ptr), _address(other._address)
    {
        other._unsafeClear();
    }

    // Copy assignment operator
    HPTR& operator=(const HPTR& other)
    {
        if (this == &other)
            return *this;

        this->_ptr = other._ptr;
        this->_address = other._address;

        this->_registerAddressMap();

        return *this;
    }

    // Move assignment operator
    HPTR& operator=(HPTR&& other) noexcept
    {
        if (this == &other)
            return *this;

        this->_ptr = other._ptr;
        this->_address = other._address;

        other._unsafeClear();

        return *this;
    }

    // Assignment operator
    HPTR& operator=(const T& value)
    {
        *this = HPTR{value};
        return *this;
    }

    HPTR& operator=(std::nullptr_t)
    {
        this->_unRegisterAddressMap();
        return *this;
    }

    // Nullptr Constructor
    HPTR() = default;

    // Constructor
    explicit HPTR(const T& value) : _ptr(new T{value}), _address(reinterpret_cast<uintptr_t>(this->_ptr))
    {
        this->_registerAddressMap();
    }

    // Destructor
    ~HPTR()
    {
        this->_unRegisterAddressMap();
    }

    // == operator
    bool operator==(std::nullptr_t) const
    {
        return this->_ptr == nullptr || HPTR::_addressMap.find(this->_address) == HPTR::_addressMap.end();
    }

    // != operator
    bool operator!=(std::nullptr_t) const
    {
        return !(*this == nullptr);
    }

    // Dereference operator
    T& operator*()
    {
        if (*this == nullptr)
            throw std::runtime_error("Tidak dapat mengakses null pointer.");

        return *_ptr;
    }

    // Arrow operator
    T* operator->()
    {
        if (*this == nullptr)
            throw std::runtime_error("Tidak dapat mengakses null pointer.");

        return _ptr;
    }

    // del
    void del()
    {
        if (*this == nullptr)
            throw std::runtime_error("Tidak dapat menghapus null pointer.");

        this->_unsafeDel();
    }

private:
    // address, instance
    static std::unordered_map<uintptr_t, std::atomic_size_t> _addressMap;

    T* _ptr{nullptr};
    uintptr_t _address{0};

    void _unsafeClear()
    {
        this->_ptr = nullptr;
        this->_address = 0;
    }

    void _unsafeDel()
    {
        HPTR::_addressMap.erase(this->_address);
        delete this->_ptr;
        _unsafeClear();
    }

    void _registerAddressMap()
    {
        if (this->_ptr == nullptr)
            return;

        auto it{HPTR::_addressMap.find(this->_address)};
        if (it == HPTR::_addressMap.end())
            HPTR::_addressMap[this->_address].store(1, std::memory_order_relaxed);
        else
            it->second.fetch_add(1, std::memory_order_relaxed);
    }

    void _unRegisterAddressMap()
    {
        auto it{HPTR::_addressMap.find(this->_address)};
        if (this->_ptr == nullptr || it == HPTR::_addressMap.end())
            return;

        if (it->second.load(std::memory_order_relaxed) > 1)
        {
            it->second.fetch_sub(1, std::memory_order_relaxed);
            this->_unsafeClear();
            return;
        }

        this->_unsafeDel();
    }
};

template <typename T>
std::unordered_map<uintptr_t, std::atomic_size_t> HPTR<T>::_addressMap{};

template <typename T>
using hptr = HPTR<T>;

} // namespace nusantara

#endif