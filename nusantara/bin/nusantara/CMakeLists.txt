# Define the executable target named "nusantara" using the source file "nusantara.cc"
add_executable(nusantara
    "nusantara.cc"
)

# Link the "nusantara" executable with the specified LLVM libraries
target_link_libraries(nusantara PRIVATE
    ${LLVMCore} # Link with LLVM Core static library
    ${LLVMSupport} # Link with LLVM Support static library
)