project(csCore LANGUAGES CXX)

# create a project assert include
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/assert/cmake/")
include(csAssert)
csAssertGenerateProject(csCore)

add_subdirectory(core)
add_subdirectory(assert)
