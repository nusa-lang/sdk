# Option to specify which Nusantara sub-projects should be built
option(NUSANTARA_PROJECTS "Option to select which Nusantara sub-projects to build")

# Iterate over each project specified in NUSANTARA_PROJECTS
foreach(PROJECT ${NUSANTARA_PROJECTS})
    # Add the specified sub-project directory to the build process
    add_subdirectory(${PROJECT})
endforeach()