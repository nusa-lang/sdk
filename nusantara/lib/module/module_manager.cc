/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/module/module_manager.h"
#include "nusantara/support/char_stream.h"
#include <queue>
#include <unordered_set>
#include <utility>

namespace nusantara {

ModuleManager::ModuleManager() = default;

void ModuleManager::push(const char* path)
{
    if (!this->_paths.contains(path))
        this->_paths.emplace(this->_queue.emplace(&this->_inputs.emplace_back(CharStream::file(path)))->path());
}

void ModuleManager::push(CharStream&& charStream)
{
    if (charStream.path() == nullptr)
    {
        this->_queue.emplace(&this->_inputs.emplace_back(std::move(charStream)));
        return;
    }

    if (!this->_paths.contains(charStream.path()))
        this->_paths.emplace(this->_queue.emplace(&this->_inputs.emplace_back(std::move(charStream)))->path());
}

void ModuleManager::push(CharStream& charStream)
{
    if (charStream.path() == nullptr)
    {
        this->_queue.emplace(&charStream);
        return;
    }

    if (!this->_paths.contains(charStream.path()))
        this->_paths.emplace(this->_queue.emplace(&charStream)->path());
}

CharStream& ModuleManager::front()
{
    return *this->_queue.front();
}

void ModuleManager::pop()
{
    this->_queue.pop();
}

bool ModuleManager::empty()
{
    return this->_queue.empty();
}

void ModuleManager::reset()
{
    while (!this->_queue.empty())
        this->_queue.pop();

    this->_paths.clear();
    this->_inputs.clear();
}

} // namespace nusantara