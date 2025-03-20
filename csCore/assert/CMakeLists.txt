project(assert LANGUAGES CXX)

cs_add_library(assert INTERFACE)
cs_add_library(csCore::assert ALIAS assert)

cs_find_package(assert PUBLIC libassert REQUIRED CONFIG)

cs_target_sources(assert 
	PRIVATE_INSTALL 
		Assert.hpp
	PRIVATE
    )

cs_target_link_libraries(assert
	INTERFACE
	csCore::csCore
	libassert::assert
)

# install the cmake file to be available for client libraries
set(SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/csAssert.cmake)
set(DEST_PATH ${CMAKE_INSTALL_PREFIX}/cmake/csCore/assert)
install(FILES ${SOURCE_PATH} DESTINATION ${DEST_PATH})
# install the template file
set(SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates/ProjectAssert.hpp.in)
set(DEST_PATH ${CMAKE_INSTALL_PREFIX}/cmake/csCore/assert/templates)
install(FILES ${SOURCE_PATH} DESTINATION ${DEST_PATH})

# install to a non default component assert
cs_install_target(assert assert)

cs_finalize_target(assert)