# Option to specify which Nusa sub-projects should be built
option(NUSA_PROJECTS "Option to select which Nusa sub-projects to build")

# Iterate over each project specified in NUSA_PROJECTS
foreach(PROJECT ${NUSA_PROJECTS})
    # Add the specified sub-project directory to the build process
    add_subdirectory(${PROJECT})
endforeach()