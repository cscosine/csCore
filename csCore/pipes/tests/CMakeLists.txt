project(pipesTest  LANGUAGES CXX)

cs_add_executable(pipesTest)

include(CTest)
cs_find_package(pipesTest PUBLIC Catch2 3 REQUIRED CONFIG)
include(Catch)

cs_target_sources(pipesTest
	PRIVATE
  dev_null.cpp
  cartesian_product.cpp
  fork.cpp
  set_aggregator.cpp
  transform.cpp
  main.cpp
  mux.cpp
  drop.cpp
  drop_while.cpp
  override.cpp
  do_then.cpp
  take_while.cpp
  tap.cpp
  tee.cpp
  filter.cpp
  for_each.cpp
  join.cpp
  intersperse.cpp
  adjacent.cpp
  unzip.cpp
  map_aggregator.cpp
  stride.cpp
  partition.cpp
  take.cpp
  integration_tests.cpp
  insert.cpp
  switch.cpp
  streams.cpp
  combinations.cpp
)

cs_target_link_libraries(pipesTest
  PRIVATE
  csCore::pipes
  Catch2::Catch2WithMain
  csCoreSol::csCoreSol_csCMakeCompileOptionsDefault
)

# add to tests
catch_discover_tests(pipesTest DISCOVERY_MODE "PRE_TEST")


cs_finalize_target(pipesTest)
