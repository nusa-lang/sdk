#!/bin/bash

# Default build type (Debug if not specified)
BUILD_TYPE="Debug"

# Function to clean and recreate the build directory
clean_build() {
    echo "Cleaning build directory..."
    rm -rf build
    mkdir build
    configure_and_build
}

# Function to configure and build the project
configure_and_build() {
    echo "Configuring project with build type: ${BUILD_TYPE}"

    # Check if Ninja is installed
    if ! command -v ninja &> /dev/null; then
        echo "Error: Ninja is not installed. Please install Ninja before proceeding."
        exit 1
    fi

    # Run CMake configuration and build
    cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=${BUILD_TYPE}
    cmake --build build
}

# Handle script arguments
if [ "$1" == "clean" ]; then
    shift # Remove "clean" from arguments

    # Check if a valid build type is provided after "clean"
    if [[ "$1" =~ ^(Debug|Release|RelWithDebInfo|MinSizeRel)$ ]]; then
        BUILD_TYPE="$1"
    fi
    clean_build
elif [[ "$1" =~ ^(Debug|Release|RelWithDebInfo|MinSizeRel)$ ]]; then
    # Set build type based on the provided argument
    BUILD_TYPE="$1"
    configure_and_build
else
    # Display usage instructions for invalid or missing arguments
    echo "Usage:"
    echo "  ./build.sh <CMAKE_BUILD_TYPE>         # Build with specified type (default: Debug)"
    echo "  ./build.sh clean [CMAKE_BUILD_TYPE]   # Clean and rebuild (default: Debug)"
    echo "Valid CMAKE_BUILD_TYPE values: Debug, Release, RelWithDebInfo, MinSizeRel"
    exit 1
fi