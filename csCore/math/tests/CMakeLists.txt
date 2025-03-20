project(mathTest  LANGUAGES CXX)

cs_add_executable(mathTest)
cs_find_package(assert PUBLIC libassert REQUIRED CONFIG)

include(CTest)
cs_find_package(mathTest PUBLIC Catch2 3 REQUIRED CONFIG)
include(Catch)

# add /Zc:preprocessor
target_compile_options(mathTest PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:/Zc:preprocessor>")  

cs_target_sources(mathTest 
	PRIVATE
  TestAnglesConversion.cpp
)

cs_target_link_libraries(mathTest 
  PRIVATE 
  csCore::math 
  Catch2::Catch2WithMain
  libassert::assert
)
  
# add to tests
catch_discover_tests(mathTest)

cs_install_target(mathTest mathTestExport)

cs_finalize_target(mathTest)