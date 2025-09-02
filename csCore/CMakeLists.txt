project(csCore LANGUAGES CXX)

# create a project assert include
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/assert/cmake/")
include(csAssert)

# set defaults for Paranoid additional configuration
csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT RELEASE Paranoid ON)
csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT RELWITHDEBINFO Paranoid ON)
csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT DEBUG Paranoid ON)
csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT PARANOID Paranoid ON)
csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT SAFE_MATH Paranoid ON)

csAssertGenerateProject(csCore csCore) #generate target csCore::csCoreAssert
list(POP_BACK CMAKE_MODULE_PATH)

cs_add_library(csCore INTERFACE)
cs_add_library(csCore::csCore ALIAS csCore)

cs_target_sources(csCore
	PRIVATE_INSTALL
	PRIVATE
)

cs_target_link_libraries(csCore
	INTERFACE
		csCoreSol::csCoreSol_csCMakeBaseInclude
)

cs_install_target(csCore csCore)

cs_finalize_target(csCore)

# add libs, executables, etc
add_subdirectory(assert)
add_subdirectory(core)
add_subdirectory(eigenUtils)
add_subdirectory(format)
add_subdirectory(formatEigen)
add_subdirectory(language)
add_subdirectory(math)
add_subdirectory(namedType)
add_subdirectory(pipes)
add_subdirectory(testUtils)
# add_subdirectory(...)
