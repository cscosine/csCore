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
		csCoreLib::csCoreLib_csCMakeBaseInclude
		csCoreLib::csCoreLib_csCMakeCompileOptionsDefault
		libassert::assert
)

# install to a non default component assert
cs_install_target(assert assert)

cs_finalize_target(assert)