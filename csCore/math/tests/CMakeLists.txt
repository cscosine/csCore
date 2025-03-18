project(mathTest  LANGUAGES CXX)

cs_add_executable(mathTest)

include(CTest)
cs_find_package(mathTest PUBLIC Catch2 3 REQUIRED CONFIG)
include(Catch)

cs_find_package(mathTest PUBLIC Eigen3 REQUIRED CONFIG)

cs_target_sources(mathTest 
	PRIVATE
  TestAnglesConversion.cpp
)

cs_target_link_libraries(mathTest 
  PRIVATE 
  csCore::math 
  csCoreLib::csCoreLib_csCMakeBaseInclude
  csCoreLib::csCoreLib_csCMakeCompileOptionsDefault
  Catch2::Catch2WithMain
  Eigen3::Eigen)
  
# add to tests
catch_discover_tests(mathTest)

cs_install_target(mathTest mathTestExport)

cs_finalize_target(mathTest)