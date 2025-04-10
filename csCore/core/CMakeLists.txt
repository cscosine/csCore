project(core LANGUAGES CXX)

cs_add_library(core INTERFACE)
cs_add_library(csCore::core ALIAS core)

cs_target_sources(core
	PRIVATE_INSTALL
		BasicTypes.hpp
	PRIVATE
    )

cs_target_link_libraries(core
	INTERFACE
		csCore::csCore
)

cs_install_target(core core)

cs_finalize_target(core)
