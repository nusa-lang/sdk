# Ensure that the project requires at least CMake version 3.28
cmake_minimum_required(VERSION 3.28)

# Define the project "Nusa SDK" with a description and specify that it uses C and C++
project("Nusa SDK"
    DESCRIPTION "The official development kit for developers using the Nusa programming language."
    LANGUAGES C CXX
)

# Prevent this file from being included multiple times globally
include_guard(GLOBAL)

# Generate a compile_commands.json file for tools like clangd and static analysis
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable position-independent code (PIC) for security and shared library support
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Check if Ninja build system is available
find_program(NINJA_EXECUTABLE ninja)

if(NINJA_EXECUTABLE)
    # If Ninja is found, set it as the default generator
    message(STATUS "Ninja build system found. Using Ninja as the default generator.")
    set(CMAKE_GENERATOR "Ninja" CACHE STRING "Preferred build system" FORCE)
else()
    # If Ninja is not found, warn the user and use the default generator
    message(WARNING "Ninja build system not found! It is recommended to use Ninja for better performance.")
endif()

# Check if ccache is available
find_program(CCACHE_EXECUTABLE ccache)

if(CCACHE_EXECUTABLE)
    # If ccache is found, enable it for faster compilation
    message(STATUS "ccache found. Enabling ccache for faster builds.")

    # Set ccache as the compiler launcher for both C and C++
    set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_EXECUTABLE}")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_EXECUTABLE}")
else()
    # If ccache is not found, warn the user
    message(WARNING "ccache not found! It is recommended to use ccache to speed up compilation.")
endif()

# Check if Clang is available
find_program(CLANG_C_COMPILER clang)
find_program(CLANG_CXX_COMPILER clang++)

if(CLANG_C_COMPILER AND CLANG_CXX_COMPILER)
    # If Clang is found, set it as the default compiler for C and C++
    message(STATUS "Clang compiler found. Using Clang as the default compiler.")

    set(CMAKE_C_COMPILER "${CLANG_C_COMPILER}" CACHE STRING "C compiler" FORCE)
    set(CMAKE_CXX_COMPILER "${CLANG_CXX_COMPILER}" CACHE STRING "C++ compiler" FORCE)

    add_compile_options(
        -stdlib=libc++
    )
else()
    # If Clang is not found, warn the user and use the default compiler
    message(WARNING "Clang compiler not found! It is recommended to use Clang for better performance and compatibility.")
endif()

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)

# Ensure that C++20 is strictly required and does not fall back to older versions
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Disable compiler-specific C++ extensions to enforce standard C++ only
set(CMAKE_CXX_EXTENSIONS OFF)

# Check if the compiler supports C++20
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++20" COMPILER_SUPPORTS_CXX20)

if(NOT COMPILER_SUPPORTS_CXX20)
    # If the compiler does not support C++20, stop with an error
    message(FATAL_ERROR "Your compiler does not support C++20! Please use a compiler that supports C++20 or newer.")
else()
    message(STATUS "C++20 support detected. Proceeding with the build.")
endif()

# Linking C ++ Standard Library and C ++ Abi Library
add_link_options(
    -lc++
    -lc++abi
)

# Check if LLD linker is available
execute_process(
    COMMAND ${CMAKE_C_COMPILER} -fuse-ld=lld --version
    RESULT_VARIABLE LLD_AVAILABLE
    OUTPUT_QUIET ERROR_QUIET
)

if(LLD_AVAILABLE EQUAL 0)
    # If LLD is available, use it as the linker
    message(STATUS "Using LLD linker (-fuse-ld=lld).")

    add_link_options(
        -fuse-ld=lld
    )
else()
    # If LLD is not available, print a warning and use the default linker
    message(WARNING "LLD linker is not available. Using the default linker instead.")
endif()

# Enable static build option (default: ON)
option(BUILD_STATIC "Enable static linking for the project" ON)

# If static build is enabled
if(BUILD_STATIC)
    # Add the '-static' flag to linker options to enforce static linking
    add_link_options(
        -static
    )

    # Print a status message indicating that static linking is enabled
    message(STATUS "Building with static linking.")
else()
    # Print a status message indicating that static linking is disabled
    message(STATUS "Building with dynamic linking.")
endif()

# Check if CMAKE_BUILD_TYPE is not set
if(NOT CMAKE_BUILD_TYPE)
    # Show a warning message to the user
    message(WARNING "CMAKE_BUILD_TYPE is not set. Defaulting to 'Debug'.")

    # Set the default build type to Debug
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

# Ensure that the build type is one of the valid options
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")

# Display the selected build type
message(STATUS "Using CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

# Apply different compile options based on the build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Debug: Enable debug symbols, disable optimizations
    add_compile_options(-g -O0 -DDEBUG)
    message(STATUS "Debug mode: Debug symbols enabled, optimizations disabled.")

elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    # Release: Enable optimizations, disable debug symbols
    add_compile_options(-O3 -DNDEBUG)
    message(STATUS "Release mode: High optimizations enabled, debug symbols disabled.")

elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    # MinSizeRel: Optimize for minimum size
    add_compile_options(-Os -DNDEBUG)
    message(STATUS "MinSizeRel mode: Optimized for minimal binary size.")

elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    # RelWithDebInfo: Optimized build with debug symbols
    add_compile_options(-O2 -g -DNDEBUG)
    message(STATUS "RelWithDebInfo mode: Optimized build with debug symbols.")

else()
    message(FATAL_ERROR "Unknown CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}. No special compile options applied.")
endif()

# Check if the build type is either "Release" or "RelWithDebInfo"
if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    # Enable Link Time Optimization (LTO) by setting interprocedural optimization
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

# Define a list of Nusa subprojects to be built
set(NUSA_PROJECTS
    nusa # Main Nusa project
)

# Include the CMake script that handles SDK versioning.
# This script sets the version numbers, build date, build time,
# and system information dynamically during the CMake configuration process.
include(cmake/sdk_version.cmake)

# Include the script for clang-format (code formatting)
include(cmake/clang_format.cmake)

# Include the script to handle Nusa subprojects
include(cmake/nusa_projects.cmake)