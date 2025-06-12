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
