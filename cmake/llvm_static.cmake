# If LLVM_STATIC_LIB_SEARCH_PATHS is not set, define default search paths
if(NOT LLVM_STATIC_LIB_SEARCH_PATHS)
    set(LLVM_STATIC_LIB_SEARCH_PATHS
        /usr/lib
        /usr/local/lib
    )
endif()

# Define the list of required static LLVM libraries
set(LLVM_STATIC_LIB_NAMES
    libLLVMCore.a
    libLLVMSupport.a
)

# Iterate over each library to check its existence
foreach(LIB ${LLVM_STATIC_LIB_NAMES})
    # Remove "lib" prefix and ".a" suffix to get the variable name
    string(REGEX REPLACE "^lib|\\.a$" "" LIB_NAME ${LIB})

    # Search for the static library in the predefined paths
    find_library(${LIB_NAME}_PATH
        NAMES ${LIB}
        PATHS ${LLVM_STATIC_LIB_SEARCH_PATHS}
        NO_DEFAULT_PATH
    )

    # If the library is not found, display an error and stop the configuration
    if(NOT ${LIB_NAME}_PATH)
        message(FATAL_ERROR "Error: ${LIB} not found in ${LLVM_STATIC_LIB_SEARCH_PATHS}.")
    endif()

    # Set the variable in the desired format (e.g., LLVMCore = /usr/local/lib/libLLVMCore.a)
    set(${LIB_NAME} ${${LIB_NAME}_PATH})
endforeach()

# Status message to confirm successful detection
message(STATUS "All required LLVM static libraries found.")