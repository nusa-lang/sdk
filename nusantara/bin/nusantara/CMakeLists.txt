# Map the required LLVM components to their corresponding library names
llvm_map_components_to_libnames(llvm_libs core)

# Define the executable target named "nusantara" using the source file "nusantara.cc"
add_executable(nusantara 
    "nusantara.cc"
)

# Link the "nusantara" executable with the specified LLVM libraries
target_link_libraries(nusantara PRIVATE
    ${llvm_libs}
)