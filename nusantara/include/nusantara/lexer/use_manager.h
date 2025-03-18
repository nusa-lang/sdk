/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_LEXER_USE_MANAGER_H
#define NUSANTARA_LEXER_USE_MANAGER_H

#include "nusantara/support/char_stream.h"
#include "nusantara/support/cstr.h"
#include <list>
#include <queue>
#include <unordered_set>

namespace nusantara {

class UseManager
{
public:
    UseManager();

    void push(const char* path);
    void push(CharStream&& charStream);
    void push(CharStream& charStream);

    CharStream& front();
    void pop();

    bool empty();

    void reset();

private:
    std::queue<CharStream*> _queue;
    std::unordered_set<const char*, CStrHash, CStrEqual> _paths;
    std::list<CharStream> _inputs;
};

} // namespace nusantara

#endif