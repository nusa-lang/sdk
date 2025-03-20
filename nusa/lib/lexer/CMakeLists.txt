# Recursively find all .cc source files in the current directory and subdirectories
file(GLOB_RECURSE NUSA_LEXER_SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cc" # Match .cc files in the current source directory
    "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cc" # Match .cc files in all subdirectories
)

# Create a library target 'NusaLexer' using the collected source files
add_library(NusaLexer ${NUSA_LEXER_SRC_FILES})