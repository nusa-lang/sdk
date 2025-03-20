/*
 * ----------------------------------------------------------------------------
 * Project: Nusa
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, The Nusa Project Authors
 * ----------------------------------------------------------------------------
 */

#ifndef NUSA_LEXER_USE_MANAGER_H
#define NUSA_LEXER_USE_MANAGER_H

#include "nusa/support/char_stream.h"
#include "nusa/support/cstr.h"
#include <list>
#include <queue>
#include <unordered_set>

namespace nusa {

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

} // namespace nusa

#endif