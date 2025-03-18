/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/lexer/use_manager.h"
#include "nusantara/support/char_stream.h"
#include <queue>
#include <unordered_set>
#include <utility>

namespace nusantara {

UseManager::UseManager() = default;

void UseManager::push(const char* path)
{
    if (!this->_paths.contains(path))
        this->_paths.emplace(this->_queue.emplace(&this->_inputs.emplace_back(CharStream::file(path)))->path());
}

void UseManager::push(CharStream&& charStream)
{
    if (charStream.path() == nullptr)
    {
        this->_queue.emplace(&this->_inputs.emplace_back(std::move(charStream)));
        return;
    }

    if (!this->_paths.contains(charStream.path()))
        this->_paths.emplace(this->_queue.emplace(&this->_inputs.emplace_back(std::move(charStream)))->path());
}

void UseManager::push(CharStream& charStream)
{
    if (charStream.path() == nullptr)
    {
        this->_queue.emplace(&charStream);
        return;
    }

    if (!this->_paths.contains(charStream.path()))
        this->_paths.emplace(this->_queue.emplace(&charStream)->path());
}

CharStream& UseManager::front()
{
    return *this->_queue.front();
}

void UseManager::pop()
{
    this->_queue.pop();
}

bool UseManager::empty()
{
    return this->_queue.empty();
}

void UseManager::reset()
{
    while (!this->_queue.empty())
        this->_queue.pop();

    this->_paths.clear();
    this->_inputs.clear();
}

} // namespace nusantara