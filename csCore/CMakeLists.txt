project(csCoreMain LANGUAGES CXX)

# create a project assert include
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/assert/cmake/")
include(csAssert)
csAssertGenerateProject(csCore csCore) #generate target csCore::csCoreAssert
list(POP_BACK CMAKE_MODULE_PATH)

# add libs, executables, etc
add_subdirectory(assert)
add_subdirectory(core)
add_subdirectory(math)
# add_subdirectory(...)
