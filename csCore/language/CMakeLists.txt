project(language LANGUAGES CXX)

cs_add_library(language INTERFACE)
cs_add_library(csCore::language ALIAS language)

cs_find_package(language PUBLIC tl-optional REQUIRED CONFIG)
cs_find_package(language PUBLIC tl-expected REQUIRED CONFIG)

cs_target_sources(language
	PRIVATE_INSTALL
		Optional.hpp
		Expected.hpp
	PRIVATE
    )

cs_target_link_libraries(language
	INTERFACE
		csCore::csCore
		tl::optional
		tl::expected
)

add_subdirectory(tests)

cs_install_target(language language)

cs_finalize_target(language)

# compile each single header in standalone, ensures includes are self contained
cs_target_check_installed_headers(language COMPILE_PROFILES cscmake_strict_warnings)
