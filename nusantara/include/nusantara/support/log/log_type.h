/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_SUPPORT_LOG_TYPE_H
#define NUSANTARA_SUPPORT_LOG_TYPE_H

#include <array>
#include <cstddef>
#include <string>

namespace nusantara {

enum LogType
{
    DEBUG,
    INFO,
    SUCCESS,
    FAILED,
    WARNING,
    ERROR
};

constexpr size_t logTypeCount()
{
    return static_cast<size_t>(LogType::ERROR) + 1;
}

static const std::array<std::string, 6> logTypeStr{"\033[38;2;176;176;176mdebug\033[0m", "\033[38;2;112;193;255minfo\033[0m", "\033[38;2;129;199;132msukses\033[0m", "\033[38;2;224;108;117mgagal\033[0m", "\033[38;2;255;195;77mperingatan\033[0m", "\033[38;2;235;87;87mkesalahan\033[0m"};

} // namespace nusantara

#endif