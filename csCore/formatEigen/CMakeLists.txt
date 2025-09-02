project(formatEigen LANGUAGES CXX)

cs_add_library(formatEigen INTERFACE)
cs_add_library(csCore::formatEigen ALIAS formatEigen)

cs_find_package(formatEigen PUBLIC fmt REQUIRED CONFIG)
cs_find_package(formatEigen PUBLIC fmt-eigen REQUIRED CONFIG)

cs_target_sources(formatEigen
	PRIVATE_INSTALL
		FormatEigen.hpp
	PRIVATE
    )

cs_target_link_libraries(formatEigen
	INTERFACE
		csCore::csCore
		csCore::format
		fmt::fmt
		fmt-eigen::fmt-eigen
)

cs_install_target(formatEigen formatEigen)

cs_finalize_target(formatEigen)

# compile each single header in standalone, ensures includes are self contained
cs_compileEachInstalledInclude(formatEigen csCoreSol::csCoreSol_csCMakeCompileOptionsStrict)
