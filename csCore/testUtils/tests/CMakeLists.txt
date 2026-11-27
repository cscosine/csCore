project(testUtilsTest LANGUAGES CXX)

include(CTest)

cs_add_executable(testUtilsTest)
cs_find_package(testUtilsTest PRIVATE Catch2 3 REQUIRED CONFIG)
include(Catch)
cs_target_sources(testUtilsTest PRIVATE TestAssertCatch2.cpp)
cs_target_link_libraries(testUtilsTest PRIVATE csCore::testUtils Catch2::Catch2WithMain)
catch_discover_tests(testUtilsTest DISCOVERY_MODE "PRE_TEST")
