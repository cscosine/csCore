project(mathTest  LANGUAGES CXX)

cs_add_executable(mathTest)

include(CTest)
cs_find_package(mathTest PUBLIC Catch2 3 REQUIRED CONFIG)
include(Catch)

cs_target_sources(mathTest
	PRIVATE
  TestAngles.cpp
)

cs_target_link_libraries(mathTest
  PRIVATE
  csCore::math
  csCore::testUtils
  Catch2::Catch2WithMain
)

# add to tests
catch_discover_tests(mathTest DISCOVERY_MODE "PRE_TEST")

cs_install_target(mathTest mathTest)

cs_finalize_target(mathTest)
