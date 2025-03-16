# Recursively find all .cc source files in the current directory and subdirectories
file(GLOB_RECURSE NUSANTARA_SUPPORT_SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cc" # Match .cc files in the current source directory
    "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cc" # Match .cc files in all subdirectories
)

# Create a library target 'NusantaraSupport' using the collected source files
add_library(NusantaraSupport ${NUSANTARA_SUPPORT_SRC_FILES})

# Link the "NusantaraSupport" library with the specified libraries
target_link_libraries(NusantaraSupport PRIVATE
    ${LLVMSupport} # Link with LLVM Support static library
)