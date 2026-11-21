project(expectedTest LANGUAGES CXX)

cs_add_executable(expectedTest)

include(CTest)
cs_find_package(expectedTest PRIVATE Catch2 3 REQUIRED CONFIG)
include(Catch)

cs_target_sources(
  expectedTest
  PRIVATE
    assertions.cpp
    assignment.cpp
    bases.cpp
    constexpr.cpp
    constructors.cpp
    emplace.cpp
    extensions.cpp
    issues.cpp
    noexcept.cpp
    observers.cpp
    relops.cpp
    swap.cpp
)

cs_target_link_libraries(expectedTest PRIVATE csCore::language Catch2::Catch2WithMain)

# add to tests
catch_discover_tests(expectedTest DISCOVERY_MODE "PRE_TEST")
