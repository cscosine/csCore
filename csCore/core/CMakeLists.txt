project(core LANGUAGES CXX)

cs_add_library(core INTERFACE)
cs_add_library(csCore::core ALIAS core)

cs_find_package(core PUBLIC tl-optional REQUIRED CONFIG)
cs_find_package(core PUBLIC tl-expected REQUIRED CONFIG)

cs_target_sources(core
	PRIVATE_INSTALL
		BasicTypes.hpp
		Optional.hpp
		Expected.hpp
	PRIVATE
    )

cs_target_link_libraries(core
	INTERFACE
		csCore::csCore
		tl::optional
		tl::expected
)

add_subdirectory(tests)

cs_install_target(core core)

cs_finalize_target(core)
