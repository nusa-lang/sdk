# Recursively find all .cc source files in the current directory and subdirectories
file(GLOB_RECURSE NUSANTARA_LEXER_SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cc" # Match .cc files in the current source directory
    "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cc" # Match .cc files in all subdirectories
)

# Create a library target 'nusantaraLexer' using the collected source files
add_library(nusantaraLexer ${NUSANTARA_LEXER_SRC_FILES})

# Link the "nusantaraLexer" library with the specified LLVM libraries
target_link_libraries(nusantaraLexer PRIVATE
    nusantaraSupport # Link with the Nusantara support library
)