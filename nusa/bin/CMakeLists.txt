# Define the executable target named "nusa" using the source file "nusa.cc"
add_executable(nusa
    "nusa.cc"
)

# Link the "nusa" executable with the specified libraries
target_link_libraries(nusa PRIVATE
    # Nusa
    NusaSupport # Link with NusaSupport static library
    NusaLexer # Link with NusaLexer static library
    NusaParser # Link with NusaParser static library
    NusaSemantic # Link with NusaSemantic static library
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(nusa PRIVATE
        ntdll # Link with ntdll.dll dynamic library
    )
endif()