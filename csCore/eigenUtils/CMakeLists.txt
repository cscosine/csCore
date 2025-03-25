project(eigenUtils LANGUAGES CXX)

cs_add_library(eigenUtils STATIC)
cs_add_library(csCore::eigenUtils ALIAS eigenUtils)

cs_find_package(eigenUtils PUBLIC Eigen3 REQUIRED CONFIG)

cs_target_sources(eigenUtils 
	PRIVATE_INSTALL 
		MatrixShape.hpp
		RotationMatrix.hpp
	PRIVATE
		MatrixShape.hpp
		RotationMatrix.cpp
    )

cs_target_link_libraries(eigenUtils
	PUBLIC
		csCore::core
		csCore::csCoreAssert
		Eigen3::Eigen
	)

add_subdirectory(tests)

cs_install_target(eigenUtils eigenUtils)

cs_finalize_target(eigenUtils)