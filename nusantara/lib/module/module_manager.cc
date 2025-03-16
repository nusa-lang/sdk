/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/module/module_manager.h"
#include "nusantara/support/input_stream.h"
#include <filesystem>
#include <queue>
#include <string>
#include <unordered_set>
#include <utility>

namespace nusantara {

ModuleManager::ModuleManager() = default;

void ModuleManager::push(std::string path)
{
    path = std::filesystem::weakly_canonical(path).string();

    if (!this->_paths.contains(path))
    {
        this->_queue.emplace(&this->_inputs.emplace_back(InputStream{path.c_str(), true}));
        this->_paths.emplace(std::move(path));
    }
}

InputStream& ModuleManager::front()
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