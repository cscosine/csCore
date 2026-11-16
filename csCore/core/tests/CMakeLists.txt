project(coreTest LANGUAGES CXX)

include(CTest)

# the OpenMP helpers, without OpenMP (serial) and with it (cs_target_use_openmp: nothing with Apple Clang)
foreach(variant serial openmp)
  cs_add_executable(coreTest_${variant})
  cs_find_package(coreTest_${variant} PRIVATE Catch2 3 REQUIRED CONFIG)
  include(Catch)
  cs_target_sources(coreTest_${variant}
    PRIVATE
    TestOpenMP.cpp
  )
  cs_target_link_libraries(coreTest_${variant}
    PRIVATE
    csCore::core
    Catch2::Catch2WithMain
  )
  catch_discover_tests(coreTest_${variant} DISCOVERY_MODE "PRE_TEST" TEST_SUFFIX " (${variant})")
endforeach()
cs_target_use_openmp(coreTest_openmp PRIVATE)
