project(csCore LANGUAGES CXX)

# create a project assert include
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/assert/cmake/")
include(csAssert)

# set defaults for Paranoid additional configuration
csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT LITE Paranoid ON)
csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT STANDARD Paranoid ON)
csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT DEFENSIVE Paranoid ON)
csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT PARANOID Paranoid ON)
csSetAssertDefaultOnForConfig(${PROJECT_NAME} CONTRACT LITE Paranoid ON)
csSetAssertDefaultOnForConfig(${PROJECT_NAME} CONTRACT STANDARD Paranoid ON)
csSetAssertDefaultOnForConfig(${PROJECT_NAME} CONTRACT DEFENSIVE Paranoid ON)
csSetAssertDefaultOnForConfig(${PROJECT_NAME} CONTRACT PARANOID Paranoid ON)

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
		csCoreLib::csCoreLib_csCMakeBaseInclude
		csCoreLib::csCoreLib_csCMakeCompileOptionsDefault
)

cs_install_target(csCore csCore)

cs_finalize_target(csCore)

# add libs, executables, etc
add_subdirectory(assert)
add_subdirectory(core)
add_subdirectory(testUtils)
add_subdirectory(math)
add_subdirectory(eigenUtils)
# add_subdirectory(...)
