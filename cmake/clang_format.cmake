# Find the clang-format executable
find_program(CLANG_FORMAT_EXECUTABLE clang-format)

# If clang-format is not found, warn the user but do not stop the build
if(NOT CLANG_FORMAT_EXECUTABLE)
    message(WARNING "clang-format not found. Code formatting will be skipped.")
else()
    # Recursively find all source (.cc) and header (.h) files in the root directories
    file(GLOB_RECURSE ALL_HEADER_AND_SRC_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/include/**/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    )
    # Iterate through each specified Nusa subproject
    foreach(PROJECT ${NUSA_PROJECTS})
        # Recursively find all source (.cc) and header (.h) files in the project directories
        file(GLOB_RECURSE PROJECT_HEADER_AND_SRC_FILES
            "${PROJECT}/bin/**/*.cc"     # Match .cc files in bin/ and its subdirectories
            "${PROJECT}/bin/*.cc"        # Match .cc files directly inside bin/
            "${PROJECT}/bin/**/*.c"     # Match .c files in bin/ and its subdirectories
            "${PROJECT}/bin/*.c"        # Match .c files directly inside bin/
            "${PROJECT}/lib/**/*.cc"     # Match .cc files in lib/ and its subdirectories
            "${PROJECT}/lib/*.cc"        # Match .cc files directly inside lib/
            "${PROJECT}/lib/**/*.c"     # Match .c files in lib/ and its subdirectories
            "${PROJECT}/lib/*.c"        # Match .c files directly inside lib/
            "${PROJECT}/test/**/*.cc"    # Match .cc files in test/ and its subdirectories
            "${PROJECT}/test/*.cc"       # Match .cc files directly inside test/
            "${PROJECT}/test/**/*.c"    # Match .c files in test/ and its subdirectories
            "${PROJECT}/test/*.c"       # Match .c files directly inside test/
            "${PROJECT}/include/**/*.h"  # Match .h files in include/ and its subdirectories
            "${PROJECT}/include/*.h"     # Match .h files directly inside include/
        )

        set(ALL_HEADER_AND_SRC_FILES 
            ${ALL_HEADER_AND_SRC_FILES}
            ${PROJECT_HEADER_AND_SRC_FILES}
        )
    endforeach()

    # Run clang-format on all found files to enforce coding style
    execute_process(COMMAND ${CLANG_FORMAT_EXECUTABLE} -i ${ALL_HEADER_AND_SRC_FILES})
endif()