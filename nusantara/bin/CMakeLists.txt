# Define the executable target named "nusantara" using the source file "nusantara.cc"
add_executable(nusantara
    "nusantara.cc"
)

# Link the "nusantara" executable with the specified libraries
target_link_libraries(nusantara PRIVATE
    NusantaraSupport # Link with Nusantara Support static library
    NusantaraLexer # Link with Nusantara Lexer static library
    NusantaraParser # Link with Nusantara Parser static library
    NusantaraSemantic # Link with Nusantara Semantic static library
)

# if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
# target_link_libraries(nusantara PRIVATE
# ntdll # Link with ntdll.dll dynamic library
# )
# endif()