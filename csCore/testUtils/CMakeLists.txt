project(testUtils LANGUAGES CXX)

cs_add_library(testUtils INTERFACE)
cs_add_library(csCore::testUtils ALIAS assert)

cs_find_package(testUtils PUBLIC libassert REQUIRED CONFIG)
cs_find_package(testUtils PUBLIC Catch2 3 REQUIRED CONFIG)

cs_target_sources(testUtils 
	PRIVATE_INSTALL 
		AssertCatch2.hpp
	PRIVATE
    )

# add /Zc:preprocessor
target_compile_options(testUtils INTERFACE "$<$<CXX_COMPILER_ID:MSVC>:/Zc:preprocessor>")  

cs_target_link_libraries(testUtils
	INTERFACE
	csCore::csCore
	libassert::assert
	Catch2::Catch2
)

# install to a non default component assert
cs_install_target(testUtils testUtils)

cs_finalize_target(testUtils)