/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#include "nusantara/support/log/log.h"
#include "nusantara/support/log/log_type.h"
#include <algorithm>
#include <array>
#include <cstddef>
#include <fstream>
#include <iosfwd>
#include <llvm/Support/raw_ostream.h>
#include <mutex>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace nusantara {

std::mutex Log::_logCountMutex{};

std::array<size_t, logTypeCount()> Log::_counts{0, 0, 0, 0, 0, 0};

std::array<std::vector<std::string>, logTypeCount()> Log::_saved{};

std::unordered_map<std::string, std::vector<std::string>> Log::_fileCache{};

void Log::reset()
{
    Log::_counts.fill(0);

    for (auto& vec : Log::_saved)
        vec.clear();

    Log::_fileCache.clear();
}

void Log::printCount()
{
    if (!Log::hasLog())
    {
        llvm::outs() << "Tidak ada log.";
        return;
    }

    llvm::outs() << "Terdapat ";
    bool init{true};
    for (size_t i = 0; i < Log::_counts.size(); i++)
    {
        if (Log::_counts[i] > 0)
        {
            if (init)
                init = false;
            else
                llvm::outs() << ", ";

            llvm::outs() << Log::_counts[i] << " " << logTypeStr[i];
        }
    }
}

void Log::printSaved()
{
    for (size_t i = 0; i < Log::_saved.size(); i++)
    {
        auto& output{(i == LogType::ERROR) ? llvm::errs() : llvm::outs()};
        for (const auto& str : Log::_saved[i])
            output << str;
    }
}

bool Log::hasLog()
{
    return std::any_of(Log::_counts.begin(), Log::_counts.end(),
        [](const auto& count)
        {
            return count > 0;
        });
}

bool Log::hasLogWithType(const LogType& type)
{
    return Log::_counts[type] > 0;
}

void Log::report()
{
    if (!Log::hasLog())
        return;

    Log::printSaved();
    Log::printCount();

    bool hasError{Log::hasError()};

    Log::reset();

    if (hasError)
        throw std::runtime_error("");
}

std::string Log::create(const LogType& type, const std::string& message)
{
    return logTypeStr[type] + ": " + message + "\n";
}

void Log::throwing(const LogType& type, const std::string& message)
{
    Log::_incrementCount(type);
    throw std::runtime_error(Log::create(type, message));
}

void Log::save(const LogType& type, const std::string& message)
{
    Log::_incrementCount(type);
    Log::_saved[type].emplace_back(Log::create(type, message));
}

void Log::print(const LogType& type, const std::string& message)
{
    Log::_incrementCount(type);
    (type == ERROR ? llvm::errs() : llvm::outs()) << Log::create(type, message);
}

std::string Log::create(const LogType& type, const std::string& source, const size_t& line, const size_t& column, const size_t& size, const std::string& message)
{
    std::string result;
    llvm::raw_string_ostream output{result};

    std::string prefix{std::to_string(line + 1) + "| "};
    size_t arrowSpace{prefix.size() + (column + 1) > 1 ? prefix.size() + (column + 1) - 1 : 0};

    output << "---------------------------------------------\n";
    output << source << ":" << line + 1 << ":" << column + 1 << " " << logTypeStr[type] << "\n\n";
    output << prefix;

    if (!source.empty())
    {
        if (!Log::_fileCache.contains(source))
            Log::_createFileCache(source);

        auto& file{Log::_fileCache[source]};
        if (!file.empty())
        {
            if (line >= file.size())
                output << "Baris " << line << " tidak ada.";
            else
            {
                output << file[line];
            }
        }
        else
            output << "Berkas tidak ada.";
    }
    else
        output << "Lokasi berkas kosong.";

    output << "\n" << std::string(arrowSpace, ' ') << std::string(size > 0 ? size : 1, '^') << "\n" << message << "\n\n";

    return output.str();
}

void Log::throwing(const LogType& type, const std::string& source, const size_t& line, const size_t& column, const size_t& size, const std::string& message)
{
    Log::_incrementCount(type);
    throw std::runtime_error(Log::create(type, source, line, column, size, message));
}

void Log::save(const LogType& type, const std::string& source, const size_t& line, const size_t& column, const size_t& size, const std::string& message)
{
    Log::_incrementCount(type);
    Log::_saved[type].emplace_back(Log::create(type, source, line, column, size, message));
}

void Log::print(const LogType& type, const std::string& source, const size_t& line, const size_t& column, const size_t& size, const std::string& message)
{
    Log::_incrementCount(type);
    (type == ERROR ? llvm::errs() : llvm::outs()) << Log::create(type, source, line, column, size, message);
}

void Log::_createFileCache(const std::string& source)
{
    std::ifstream file{source};
    if (!file)
    {
        Log::_fileCache[source] = {};
        return;
    }

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file, line))
        lines.emplace_back(line);

    Log::_fileCache[source] = std::move(lines);
}

void Log::_incrementCount(const LogType& type)
{
    std::lock_guard<std::mutex> lock{nusantara::Log::_logCountMutex};
    nusantara::Log::_counts[static_cast<size_t>(type)]++;
}

} // namespace nusantara