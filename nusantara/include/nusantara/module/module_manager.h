/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_MODULE_MODULE_MANAGER_H
#define NUSANTARA_MODULE_MODULE_MANAGER_H

#include "nusantara/support/input_stream.h"
#include <list>
#include <queue>
#include <string>
#include <unordered_set>

namespace nusantara {

class ModuleManager
{
public:
    ModuleManager();

    void push(std::string path);

    InputStream& front();
    void pop();

    bool empty();

    void reset();

private:
    std::queue<InputStream*> _queue;
    std::unordered_set<std::string> _paths;
    std::list<InputStream> _inputs;
};

} // namespace nusantara

#endif