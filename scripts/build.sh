#!/bin/bash

# Default build type (Debug if not specified)
BUILD_TYPE="Debug"
UNIT_TEST="OFF"

# Function to clean and recreate the build directory
clean_build() {
    echo "Cleaning build directory..."
    rm -rf build
    mkdir build
    configure_and_build
}

# Function to configure and build the project
configure_and_build() {
    echo "Configuring project with build type: ${BUILD_TYPE} and unit test: ${UNIT_TEST}"
    
    # Check if Ninja is available
    if command -v ninja &> /dev/null; then
        GENERATOR="-G Ninja"
    else
        GENERATOR=""
        echo "Warning: Ninja is not installed. Using default CMake generator, but it's recommended to use Ninja for better performance."
    fi
    
    # Run CMake configuration and build
    cmake -S . -B build $GENERATOR -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DUNIT_TEST=${UNIT_TEST}
    cmake --build build
}

# Parse script arguments
while [[ $# -gt 0 ]]; do
    case "$1" in
        --clean)
            CLEAN=true
            ;;
        --unit-test)
            UNIT_TEST="ON"
            ;;
        --debug|--release|--relwithdebinfo|--minsizerel)
            # Convert lowercase argument to proper CMake build type format
            BUILD_TYPE=$(echo "$1" | sed 's/--//; s/.*/\u&/')
            ;;
        *)
            echo "Invalid argument: $1"
            echo "Usage:"
            echo "  ./build.sh [--clean] [--unit-test] [--debug|--release|--relwithdebinfo|--minsizerel]"
            echo "  --clean        : Remove the build folder before rebuilding"
            echo "  --unit-test    : Enable unit tests (-DUNIT_TEST=ON)"
            echo "  --debug        : Set CMake build type to Debug"
            echo "  --release      : Set CMake build type to Release"
            echo "  --relwithdebinfo : Set CMake build type to RelWithDebInfo"
            echo "  --minsizerel   : Set CMake build type to MinSizeRel"
            exit 1
            ;;
    esac
    shift
done

# Execute clean build if --clean was provided
if [ "$CLEAN" = true ]; then
    clean_build
else
    configure_and_build
fi