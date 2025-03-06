/*
 * ----------------------------------------------------------------------------
 * Project: Nusantara
 * Author: Fern Aerell
 * License: BSD 3-Clause License
 * Copyright (c) 2025, Nusantara
 * ----------------------------------------------------------------------------
 */

#ifndef NUSANTARA_SUPPORT_LOG_H
#define NUSANTARA_SUPPORT_LOG_H

#include "nusantara/support/log/log_type.h"
#include <array>
#include <cstddef>
#include <iosfwd>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace nusantara {

class Log
{
public:
    static void reset();
    static void printCount();
    static void printSaved();
    static bool hasLog();
    static bool hasLogWithType(const LogType& type);

#define NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_HAS_LOG_WITH_TYPE_SHORTCUT(name, type) \
inline static bool has##name() \
{ \
    return Log::_counts[LogType::type] > 0; \
}
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_HAS_LOG_WITH_TYPE_SHORTCUT(Debug, DEBUG)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_HAS_LOG_WITH_TYPE_SHORTCUT(Info, INFO)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_HAS_LOG_WITH_TYPE_SHORTCUT(Success, SUCCESS)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_HAS_LOG_WITH_TYPE_SHORTCUT(Failed, FAILED)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_HAS_LOG_WITH_TYPE_SHORTCUT(Warning, WARNING)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_HAS_LOG_WITH_TYPE_SHORTCUT(Error, ERROR)

    static void report();

    static std::string create(const LogType& type, const std::string& message);
    static void throwing(const LogType& type, const std::string& message);
    static void save(const LogType& type, const std::string& message);
    static void print(const LogType& type, const std::string& message);
    static std::string create(const LogType& type, const std::string& source, const size_t& line, const size_t& column, const size_t& size, const std::string& message);
    static void throwing(const LogType& type, const std::string& source, const size_t& line, const size_t& column, const size_t& size, const std::string& message);
    static void save(const LogType& type, const std::string& source, const size_t& line, const size_t& column, const size_t& size, const std::string& message);
    static void print(const LogType& type, const std::string& source, const size_t& line, const size_t& column, const size_t& size, const std::string& message);

#define NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_PRINT_SHORTCUT(name, type) \
inline static void name(const std::string& message) {\
    Log::print(LogType::type, message);\
}
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_PRINT_SHORTCUT(debug, DEBUG)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_PRINT_SHORTCUT(info, INFO)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_PRINT_SHORTCUT(success, SUCCESS)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_PRINT_SHORTCUT(failed, FAILED)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_PRINT_SHORTCUT(warning, WARNING)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_PRINT_SHORTCUT(error, ERROR)

#define NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_SAVE_SHORTCUT(name, type) \
static void save##name(const std::string& message) {\
    Log::save(LogType::type, message);\
}
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_SAVE_SHORTCUT(Debug, DEBUG)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_SAVE_SHORTCUT(Info, INFO)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_SAVE_SHORTCUT(Success, SUCCESS)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_SAVE_SHORTCUT(Failed, FAILED)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_SAVE_SHORTCUT(Warning, WARNING)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_SAVE_SHORTCUT(Error, ERROR)

#define NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_THROW_SHORTCUT(name, type) \
inline static void throw##name(const std::string& message) \
{ \
    Log::throwing(LogType::type, message); \
}
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_THROW_SHORTCUT(Debug, DEBUG)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_THROW_SHORTCUT(Info, INFO)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_THROW_SHORTCUT(Success, SUCCESS)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_THROW_SHORTCUT(Failed, FAILED)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_THROW_SHORTCUT(Warning, WARNING)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_THROW_SHORTCUT(Error, ERROR)

#define NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_PRINT_SHORTCUT2(name, type) \
inline static void name(const std::string& source, const size_t& line, const size_t& column, const size_t& size, const std::string& message) \
{\
    Log::print(LogType::type, source, line, column, size, message);\
}
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_PRINT_SHORTCUT2(debug, DEBUG)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_PRINT_SHORTCUT2(info, INFO)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_PRINT_SHORTCUT2(success, SUCCESS)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_PRINT_SHORTCUT2(failed, FAILED)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_PRINT_SHORTCUT2(warning, WARNING)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_PRINT_SHORTCUT2(error, ERROR)

#define NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_SAVE_SHORTCUT2(name, type) \
inline static void save##name(const std::string& source, const size_t& line, const size_t& column, const size_t& size, const std::string& message) \
{\
    Log::save(LogType::type, source, line, column, size, message);\
}
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_SAVE_SHORTCUT2(Debug, DEBUG)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_SAVE_SHORTCUT2(Info, INFO)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_SAVE_SHORTCUT2(Success, SUCCESS)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_SAVE_SHORTCUT2(Failed, FAILED)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_SAVE_SHORTCUT2(Warning, WARNING)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_SAVE_SHORTCUT2(Error, ERROR)

#define NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_THROW_SHORTCUT2(name, type) \
inline static void throw##name(const std::string& source, const size_t& line, const size_t& column, const size_t& size, const std::string& message) \
{\
    Log::throwing(LogType::type, source, line, column, size, message);\
}
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_THROW_SHORTCUT2(Debug, DEBUG)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_THROW_SHORTCUT2(Info, INFO)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_THROW_SHORTCUT2(Success, SUCCESS)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_THROW_SHORTCUT2(Failed, FAILED)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_THROW_SHORTCUT2(Warning, WARNING)
    NUSANTARA_SUPPORT_LOG_H_STATIC_FUNCTION_THROW_SHORTCUT2(Error, ERROR)

private:
    static std::mutex _logCountMutex;
    static std::array<size_t, logTypeCount()> _counts;
    static std::array<std::vector<std::string>, logTypeCount()> _saved;
    static std::unordered_map<std::string, std::vector<std::string>> _fileCache;

    static void _createFileCache(const std::string& source);
    static void _incrementCount(const LogType& type);
};

} // namespace nusantara

#endif