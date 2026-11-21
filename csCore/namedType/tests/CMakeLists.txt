project(namedTypeTest LANGUAGES CXX)

cs_add_executable(namedTypeTest)

include(CTest)
cs_find_package(namedTypeTest PRIVATE Catch2 3 REQUIRED CONFIG)
include(Catch)

cs_target_sources(namedTypeTest PRIVATE TestNamedType.cpp)

cs_target_link_libraries(namedTypeTest PRIVATE csCore::namedType Catch2::Catch2WithMain)

# add to tests
catch_discover_tests(namedTypeTest DISCOVERY_MODE "PRE_TEST")
