project(optionalTest LANGUAGES CXX)

cs_add_executable(optionalTest)

include(CTest)
cs_find_package(optionalTest PRIVATE Catch2 3 REQUIRED CONFIG)
include(Catch)

cs_target_sources(
  optionalTest
  PRIVATE
    assignment.cpp
    bases.cpp
    constexpr.cpp
    constructors.cpp
    emplace.cpp
    extensions.cpp
    hash.cpp
    in_place.cpp
    issues.cpp
    make_optional.cpp
    noexcept.cpp
    nullopt.cpp
    observers.cpp
    relops.cpp
    swap.cpp
)

cs_target_link_libraries(optionalTest PRIVATE csCore::language Catch2::Catch2WithMain)

# add to tests
catch_discover_tests(optionalTest DISCOVERY_MODE "PRE_TEST")
