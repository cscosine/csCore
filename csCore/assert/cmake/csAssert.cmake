include_guard(GLOBAL)
include(GNUInstallDirs)

# The assertion levels of a project: a cache option per level (and per configuration with a multi-config generator),
# a generated header <project>Assert.hpp and an INTERFACE target <project>Assert with the compile definitions of the
# levels enabled. Changing a default (cs_assert_set_default_on_for_config) needs a fresh build directory: the options
# are already in the cache.

# Set out_var to the assertion levels (ASSERT topic): a new level here and in templates/ProjectAssert.hpp.in.
function(_cs_assert_levels out_var)
  set(
    ${out_var}
    RELEASE
    RELWITHDEBINFO
    DEBUG
    PARANOID
    SAFE_MATH
    PARENT_SCOPE
  )
endfunction()

# Set out_var to the name of a define: <PROJECT>_<topic>_<name>.
function(_cs_assert_define_name upper_project topic name out_var)
  set(${out_var} "${upper_project}_${topic}_${name}" PARENT_SCOPE)
endfunction()

# Set out_var to the name of the option / compile definition enabling a level: <PROJECT>_<topic>_<name>_ENABLED.
function(_cs_assert_define_enabled_name upper_project topic name out_var)
  set(${out_var} "${upper_project}_${topic}_${name}_ENABLED" PARENT_SCOPE)
endfunction()

# Set the default (ON / OFF) of an assertion level of a project for a configuration, in the cache.
function(
  cs_assert_set_default_on_for_config
  project_name
  topic
  name
  config
  value
)
  string(TOUPPER ${project_name} upper_project)
  set(
    ${upper_project}_${topic}_${name}_CONFIG_${config}_DEFAULT
    "${value}"
    CACHE BOOL
    "${topic} ${name} default value"
    FORCE
  )
endfunction()

# The default levels of the standard configurations: Release enables RELEASE; Debug RELEASE, RELWITHDEBINFO and DEBUG;
# RelWithDebInfo RELEASE and RELWITHDEBINFO.
function(_cs_assert_set_standard_defaults project_name)
  _cs_assert_levels(levels)
  foreach(config Release Debug RelWithDebInfo)
    if(config STREQUAL "Release")
      set(enabled RELEASE)
    elseif(config STREQUAL "Debug")
      set(enabled RELEASE RELWITHDEBINFO DEBUG)
    else()
      set(enabled RELEASE RELWITHDEBINFO)
    endif()
    foreach(level ${levels})
      set(value OFF)
      if(level IN_LIST enabled)
        set(value ON)
      endif()
      cs_assert_set_default_on_for_config(${project_name} ASSERT ${level} ${config} ${value})
    endforeach()
  endforeach()
endfunction()

# A cache option per assertion level, initialised from the defaults: per configuration (<option>_CONFIG_<config>) with
# a multi-config generator, of CMAKE_BUILD_TYPE otherwise.
function(_cs_assert_create_options upper_project)
  _cs_assert_levels(levels)
  get_property(multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
  foreach(level ${levels})
    _cs_assert_define_enabled_name(${upper_project} ASSERT ${level} option_name)
    _cs_assert_define_name(${upper_project} ASSERT ${level} base_name)
    if(multi_config)
      foreach(config ${CMAKE_CONFIGURATION_TYPES})
        set(default_name "${base_name}_CONFIG_${config}_DEFAULT")
        if(NOT DEFINED ${default_name})
          message(
            FATAL_ERROR
            "no default value for configuration ${config}: ${default_name} does not exist; call "
            "cs_assert_set_default_on_for_config for ${config} before cs_assert_generate_project"
          )
        endif()
        option(
          ${option_name}_CONFIG_${config}
          "enable definition of ${option_name} for config ${config}"
          ${${default_name}}
        )
      endforeach()
    else()
      set(default_value OFF)
      if(${base_name}_CONFIG_${CMAKE_BUILD_TYPE}_DEFAULT)
        set(default_value ON)
      endif()
      option(${option_name} "enable definition of ${option_name}" ${default_value})
    endif()
  endforeach()
endfunction()

# Set out_var to the configurations an assertion level is enabled in: the options with a multi-config generator; with
# a single-config one, the option of CMAKE_BUILD_TYPE and the defaults of the other configurations (every configuration
# is installed in the same package, one targets file: the definitions must not depend on the last one installed).
function(_cs_assert_enabled_configs upper_project level out_var)
  _cs_assert_define_enabled_name(${upper_project} ASSERT ${level} option_name)
  _cs_assert_define_name(${upper_project} ASSERT ${level} base_name)
  set(configs "")
  get_property(multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
  if(multi_config)
    foreach(config ${CMAKE_CONFIGURATION_TYPES})
      if(${${option_name}_CONFIG_${config}})
        list(APPEND configs ${config})
      endif()
    endforeach()
  else()
    if(${${option_name}})
      list(APPEND configs ${CMAKE_BUILD_TYPE})
    endif()
    get_cmake_property(cache_variables CACHE_VARIABLES)
    foreach(variable ${cache_variables})
      if(variable MATCHES "^${base_name}_CONFIG_(.+)_DEFAULT$")
        if(NOT CMAKE_MATCH_1 STREQUAL CMAKE_BUILD_TYPE AND ${variable})
          list(APPEND configs ${CMAKE_MATCH_1})
        endif()
      endif()
    endforeach()
  endif()
  set(${out_var} ${configs} PARENT_SCOPE)
endfunction()

# Set out_var to the compile definitions of the levels enabled: $<$<CONFIG:c1,c2,...>:<define>>, evaluated in the
# configuration of the build, also for the consumers of the installed package (in their own configuration).
function(_cs_assert_compile_definitions upper_project out_var)
  _cs_assert_levels(levels)
  set(definitions "")
  foreach(level ${levels})
    _cs_assert_define_enabled_name(${upper_project} ASSERT ${level} define)
    _cs_assert_enabled_configs(${upper_project} ${level} configs)
    if(configs)
      list(SORT configs)
      list(JOIN configs "," configs)
      list(APPEND definitions "$<$<CONFIG:${configs}>:${define}>")
    endif()
  endforeach()
  set(${out_var} ${definitions} PARENT_SCOPE)
endfunction()

# Generate the assertion header of a project and its INTERFACE target <project>Assert (alias
# <namespace>::<project>Assert): the levels as cache options, the header (the same for every configuration: the
# levels enabled are the compile definitions of the target, per configuration), libassert, installed in its own
# component.
function(cs_assert_generate_project project_name target_namespace)
  string(TOUPPER ${project_name} upper_project)
  _cs_assert_set_standard_defaults(${project_name})
  _cs_assert_create_options(${upper_project})
  _cs_assert_compile_definitions(${upper_project} definitions)

  # the variables of the template
  set(cs_assert_namespace ${project_name})
  set(cs_assert_project_name ${upper_project})
  set(relative_path ${CMAKE_CURRENT_SOURCE_DIR})
  cmake_path(RELATIVE_PATH relative_path BASE_DIRECTORY ${CMAKE_SOURCE_DIR})
  cs_get_generated_dir(generated_folder)
  set(generated_file "${generated_folder}/${relative_path}/${project_name}Assert.hpp")
  configure_file("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/templates/ProjectAssert.hpp.in" "${generated_file}" @ONLY)

  set(target ${project_name}Assert)
  cs_add_library(${target} INTERFACE)
  cs_add_library(${target_namespace}::${target} ALIAS ${target})
  cs_find_package(${target} PUBLIC libassert REQUIRED CONFIG)
  cs_target_sources(${target} PRIVATE_INSTALL_ABSOLUTE_PATH ${generated_file} PRIVATE)
  cs_target_link_libraries(${target} INTERFACE libassert::assert)
  target_compile_definitions(${target} INTERFACE ${definitions})
  target_include_directories(${target} INTERFACE $<BUILD_INTERFACE:${generated_folder}>)
  cs_install_target(${target} COMPONENT ${target} DEFAULT)
  # compile each single header in standalone, ensures includes are self contained
  cs_target_check_installed_headers(${target} COMPILE_PROFILES cscmake_strict_warnings)
endfunction()
