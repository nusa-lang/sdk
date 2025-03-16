# Recursively find all .cc source files in the current directory and subdirectories
file(GLOB_RECURSE NUSANTARA_MODULE_SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cc" # Match .cc files in the current source directory
    "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cc" # Match .cc files in all subdirectories
)

# Create a library target 'NusantaraModule' using the collected source files
add_library(NusantaraModule ${NUSANTARA_MODULE_SRC_FILES})