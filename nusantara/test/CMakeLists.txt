# Recursively find all .cc source files in the current directory and subdirectories
file(GLOB_RECURSE NUSANTARA_TEST_SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cc" # Match .cc files in the current source directory
    "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cc" # Match .cc files in all subdirectories
)

# Create an executable target 'nusantara_test' using the collected source files
add_executable(nusantara_test ${NUSANTARA_TEST_SRC_FILES})

# Link 'nusantara_test' with necessary libraries
target_link_libraries(nusantara_test PRIVATE
    nusantaraLexer # Link with the Nusantara lexer library
    nusantaraSupport # Link with the Nusantara support library
    gtest # Link with Google Test framework
    gtest_main # Link with Google Test's main function
)