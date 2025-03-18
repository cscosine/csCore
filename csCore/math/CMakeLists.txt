project(math LANGUAGES CXX)

cs_add_library(math STATIC)
cs_add_library(csCore::math ALIAS math)

cs_target_sources(math 
	PRIVATE_INSTALL 
		AnglesConversion.hpp
	PRIVATE
		AnglesConversion.cpp
    )

cs_target_link_libraries(math
	PUBLIC
		csCoreLib::csCoreLib_csCMakeBaseInclude
		csCoreLib::csCoreLib_csCMakeCompileOptionsDefault
	PRIVATE
		csCore::csCoreAssert
		csCore::core
)

add_subdirectory(tests)

cs_install_target(math math)

cs_finalize_target(math)