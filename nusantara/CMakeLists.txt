# Add the "include" directory to the list of directories to be searched for header files
include_directories("include")

# Add the "bin" subdirectory to the build process
add_subdirectory(bin)

# Add the "lib" subdirectory to the build process
add_subdirectory(lib)

# Check if UNIT_TEST is set to ON
# If enabled, the test folder will be included in the build
if(UNIT_TEST STREQUAL "ON")
    # Add the "test" subdirectory to the build process
    add_subdirectory(test)
endif()