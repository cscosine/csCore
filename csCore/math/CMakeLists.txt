project(math LANGUAGES CXX)

cs_add_library(math STATIC)
cs_add_library(csCore::math ALIAS math)
cs_exec_clang_tidy(math)

cs_target_sources(math
	PRIVATE_INSTALL
		Angles.hpp
	PRIVATE
		Angles.cpp
    )

set_source_files_properties(Angles.cpp PROPERTIES COMPILE_OPTIONS "$<$<AND:$<PLATFORM_ID:Windows>,$<CXX_COMPILER_ID:Clang>>:-Wno-reserved-macro-identifier;-Wall>")


cs_target_link_libraries(math
	PUBLIC
		csCore::core
		csCore::csCoreAssert
	PRIVATE
  		csCoreSol::csCoreSol_csCMakeCompileOptionsStrict
	)

add_subdirectory(tests)

cs_install_target(math math)

cs_finalize_target(math)

# compile each single header in standalone, ensures includes are self contained
cs_compileEachInstalledInclude(math csCoreSol::csCoreSol_csCMakeCompileOptionsStrict)
