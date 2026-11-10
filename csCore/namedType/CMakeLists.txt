project(namedType LANGUAGES CXX)

cs_add_library(namedType INTERFACE)
cs_add_library(csCore::namedType ALIAS namedType)

cs_find_package(namedType PUBLIC NamedType REQUIRED CONFIG)

cs_target_sources(namedType
	PRIVATE_INSTALL
		NamedType.hpp
	PRIVATE
    )

cs_target_link_libraries(namedType
	INTERFACE
		csCore::csCore
		NamedType
	)

add_subdirectory(tests)

cs_install_target(namedType namedType)

cs_finalize_target(namedType)

# compile each single header in standalone, ensures includes are self contained
cs_target_check_installed_headers(namedType COMPILE_PROFILES cscmake_strict_warnings)
