project(csCore LANGUAGES CXX)

# create a project assert include
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/assert/cmake/")
include(csAssert)

# set defaults for Paranoid additional configuration
cs_assert_set_default_on_for_config(${PROJECT_NAME} ASSERT RELEASE Paranoid ON)
cs_assert_set_default_on_for_config(${PROJECT_NAME} ASSERT RELWITHDEBINFO Paranoid ON)
cs_assert_set_default_on_for_config(${PROJECT_NAME} ASSERT DEBUG Paranoid ON)
cs_assert_set_default_on_for_config(${PROJECT_NAME} ASSERT PARANOID Paranoid ON)
cs_assert_set_default_on_for_config(${PROJECT_NAME} ASSERT SAFE_MATH Paranoid ON)

cs_assert_generate_project(csCore csCore) #generate target csCore::csCoreAssert
list(POP_BACK CMAKE_MODULE_PATH)

cs_add_library(csCore INTERFACE)
cs_add_library(csCore::csCore ALIAS csCore)

cs_target_sources(csCore PRIVATE_INSTALL)

cs_target_link_libraries(csCore INTERFACE csCore::cscmake_base_include)

cs_install_target(csCore COMPONENT csCore)

# add libs, executables, etc
add_subdirectory(assert)
add_subdirectory(core)
add_subdirectory(eigenUtils)
add_subdirectory(format)
add_subdirectory(formatEigen)
add_subdirectory(language)
add_subdirectory(math)
add_subdirectory(namedType)
add_subdirectory(testUtils)
# add_subdirectory(...)
