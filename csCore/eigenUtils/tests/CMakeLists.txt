project(eigenUtilsTest  LANGUAGES CXX)

cs_add_executable(eigenUtilsTest)

include(CTest)
cs_find_package(eigenUtilsTest PUBLIC Catch2 3 REQUIRED CONFIG)
include(Catch)

cs_target_sources(eigenUtilsTest 
	PRIVATE
  TestMatrixShape.cpp
  TestRotationMatrix.cpp
)

cs_target_link_libraries(eigenUtilsTest 
  PRIVATE 
  csCore::core
  csCore::eigenUtils 
  csCore::testUtils
  Catch2::Catch2WithMain
)
  
# add to tests
catch_discover_tests(eigenUtilsTest)

cs_install_target(eigenUtilsTest eigenUtilsTestExport)

cs_finalize_target(eigenUtilsTest)