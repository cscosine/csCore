project(csCore LANGUAGES CXX)

cs_add_library(csCore INTERFACE)
cs_add_library(csCore::csCore ALIAS csCore)


# use the target_sources_wrapper, which wrap cmake native target_sources. 
# Wrapping is necessary to cover both build and installation step
# Normally headers are PUBLIC and cpp are PRIVATE, but you might have private headers too
# If the option NO_AUTO_SOURCE_GROUP is omitted the target sources and headers are 
# automatically grouped respectively in "Sources" and "Headers" folder in the IDE
# environment whereas the dependency headers (i.e. those from linked targets) are grouped
# in the "Dependency Headers" folder.
# Specify NO_AUTO_SOURCE_GROUP if you want all sources and headers grouped in the IDE
# standard folders ("Source Files" and "Header Files" in Visual Studio).
# note: you can repeat target_sources_wrapper if you want to organize your files
cs_target_sources(csCore 
	PRIVATE_INSTALL 
		BasicTypes.hpp
	PRIVATE
    )

cs_target_link_libraries(csCore
	INTERFACE
		csCoreLib::csCoreLibBaseInclude
)

cs_install_target(csCore csCore DEFAULT)

cs_finalize_target(csCore)