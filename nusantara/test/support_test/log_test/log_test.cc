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
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

class LogTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        nusantara::Log::reset();
    }
};

TEST_F(LogTest, ResetClearsLogs)
{
    nusantara::Log::save(nusantara::LogType::INFO, "Info log");
    ASSERT_TRUE(nusantara::Log::hasLog());
    nusantara::Log::reset();
    ASSERT_FALSE(nusantara::Log::hasLog());
}

TEST_F(LogTest, HasLogWithType)
{
    nusantara::Log::save(nusantara::LogType::WARNING, "Warning log");
    ASSERT_TRUE(nusantara::Log::hasLogWithType(nusantara::LogType::WARNING));
    ASSERT_FALSE(nusantara::Log::hasLogWithType(nusantara::LogType::ERROR));
}

TEST_F(LogTest, SaveLogIncreasesCount)
{
    nusantara::Log::save(nusantara::LogType::ERROR, "Error log");
    ASSERT_TRUE(nusantara::Log::hasError());
}

TEST_F(LogTest, ThrowingLogThrowsException)
{
    EXPECT_THROW(nusantara::Log::throwing(nusantara::LogType::ERROR, "Critical error"), std::runtime_error);
}

TEST_F(LogTest, CreateLogFormatsCorrectly)
{
    std::string message = "Test log message";
    std::string expected = nusantara::logTypeStr[nusantara::LogType::INFO] + ": " + message + "\n";
    ASSERT_EQ(nusantara::Log::create(nusantara::LogType::INFO, message), expected);
}