# Recursively find all .cc source files in the current directory and subdirectories
file(GLOB_RECURSE NUSA_SEMANTIC_SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cc" # Match .cc files in the current source directory
    "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cc" # Match .cc files in all subdirectories
)

# Create a library target 'NusaSemantic' using the collected source files
add_library(NusaSemantic ${NUSA_SEMANTIC_SRC_FILES})