cmake_minimum_required(VERSION 3.31)
project(csCoreLib VERSION 1.0.0 LANGUAGES CXX)

# add the csCMake system
include(csCMake)

cs_initialize_project(csCoreLib)

# create a project assert include
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/csCore/assert/cmake/")
include(csAssert)
csAssertGenerateProject(csCore)

# add libs, executables, etc
add_subdirectory(csCore/assert)
add_subdirectory(csCore/core)
# add_subdirectory(...)

# remember to add this at the end of the main cmake file
cs_finalize_project(csCore)
