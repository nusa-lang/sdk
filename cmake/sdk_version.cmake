# Manually set version by the developer
set(SDK_MAJOR_VERSION 0)
set(SDK_MINOR_VERSION 0)
set(SDK_PATCH_VERSION 0)

# Manually set release type by the developer
set(SDK_VERSION_TYPE "dev")

# Get the build date and time
string(TIMESTAMP RAW_DATE "%Y-%m-%d")
string(TIMESTAMP SDK_BUILD_TIME "%H:%M:%S")

# Format the date into a human-readable format
set(DAY_NAMES "Sunday;Monday;Tuesday;Wednesday;Thursday;Friday;Saturday")
string(TIMESTAMP DAY_INDEX "%w")
list(GET DAY_NAMES ${DAY_INDEX} DAY_NAME)

set(MONTH_NAMES "Jan;Feb;Mar;Apr;May;Jun;Jul;Aug;Sep;Oct;Nov;Dec")
string(TIMESTAMP MONTH_INDEX "%m")
math(EXPR MONTH_INDEX "${MONTH_INDEX} - 1")
list(GET MONTH_NAMES ${MONTH_INDEX} MONTH_NAME)

string(TIMESTAMP DAY "%d")
string(TIMESTAMP YEAR "%Y")

set(SDK_BUILD_DATE "${DAY_NAME}, ${DAY} ${MONTH_NAME} ${YEAR}")

# Target platform
set(SDK_BUILD_SYSTEM_NAME ${CMAKE_SYSTEM_NAME})
set(SDK_BUILD_SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR})

# Configure the version header file from the template
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/sdk/version.h.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/sdk/version.h"
    @ONLY
)

# Display build information
message(STATUS "SDK Version: ${SDK_MAJOR_VERSION}.${SDK_MINOR_VERSION}.${SDK_PATCH_VERSION} (${SDK_VERSION_TYPE})")
message(STATUS "Build Date: ${SDK_BUILD_DATE}")
message(STATUS "Build Time: ${SDK_BUILD_TIME}")
message(STATUS "Target System: ${SDK_BUILD_SYSTEM_NAME} (${SDK_BUILD_SYSTEM_PROCESSOR})")