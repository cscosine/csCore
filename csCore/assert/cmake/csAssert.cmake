include_guard(GLOBAL)
include(GNUInstallDirs)

function(csAssertGenerateProject PROJECT_NAME)
    string(TOUPPER ${PROJECT_NAME} CS_ASSERT_PROJECT_NAME)

    # list of accepted values
    set(LEVEL_VALUES NONE MINIMAL STANDARD DEFENSIVE PARANOID)

    # list of default known mapping
    set(${PROJECT_NAME}_CS_ASSERT_LEVEL_DEFAULT_Debug DEFENSIVE)
    set(${PROJECT_NAME}_CS_ASSERT_LEVEL_DEFAULT_Release NONE)
    set(${PROJECT_NAME}_CS_ASSERT_LEVEL_DEFAULT_RelWithDebInfo MINIMAL)

    get_property(GENERATOR_IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if(GENERATOR_IS_MULTI_CONFIG)

        set(CS_ASSERT_LEVEL "") 
        # multi config, create all single variables and add a default level based on the default defined above,
        foreach(CONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})
            set(DEFAULT_LEVEL_VARIABLE_NAME ${PROJECT_NAME}_CS_ASSERT_LEVEL_DEFAULT_${CONFIG_TYPE})
            if(NOT DEFINED ${DEFAULT_LEVEL_VARIABLE_NAME})
                message(FATAL_ERROR "csAssertGenerateProject asked to define a default assert level for config ${CONFIG_TYPE}, but a variable named ${DEFAULT_LEVEL_VARIABLE_NAME} does not exists. Add one default to your configuration or presets")
            endif()
            # create the configurable cache value for this build and assign to its default
            set(${PROJECT_NAME}_CS_ASSERT_LEVEL_CONFIG_${CONFIG_TYPE} "${${DEFAULT_LEVEL_VARIABLE_NAME}}"
                CACHE STRING
                "Configure the assert level for ${PROJECT_NAME}, use one of ${LEVEL_VALUES}")

            # ensure assert level is valid
            _cs_assert_value_is_in_list("${${PROJECT_NAME}_CS_ASSERT_LEVEL_CONFIG_${CONFIG_TYPE}}" "${LEVEL_VALUES}" "${PROJECT_NAME}_CS_ASSERT_LEVEL_CONFIG_${CONFIG_TYPE}")

            # and add it to a generator expression
            set(CS_ASSERT_LEVEL "${CS_ASSERT_LEVEL}$<$<CONFIG:${CONFIG_TYPE}>:${${PROJECT_NAME}_CS_ASSERT_LEVEL_CONFIG_${CONFIG_TYPE}}>")
        endforeach()        

        # first, configure a file in a common directory
        cs_get_generate_folder_common(COMMON_FOLDER)
        configure_file(
          "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../templates/ProjectAssert.hpp.in"
          "${COMMON_FOLDER}/${PROJECT_NAME}Assert.hpp.in"
        )

        # then generate, to resolve generator expression according to the specific config
        cs_get_generate_folder_config(SPECIFIC_FOLDER)
        file(GENERATE
          OUTPUT "${SPECIFIC_FOLDER}/${PROJECT_NAME}Assert.hpp"
          INPUT "${COMMON_FOLDER}/${PROJECT_NAME}Assert.hpp.in"
        )

    else()
        # single config, create a single variable and add a default level based on CMAKE_BUILD_TYPE
        # if the default for the requested CMAKE_BUILD_TYPE does not exist, terminate with error and inform the user 
        set(DEFAULT_LEVEL_VARIABLE_NAME ${PROJECT_NAME}_CS_ASSERT_LEVEL_DEFAULT_${CMAKE_BUILD_TYPE})
        if(NOT DEFINED ${DEFAULT_LEVEL_VARIABLE_NAME})
            message(FATAL_ERROR "csAssertGenerateProject asked to define a default assert level for config ${CMAKE_BUILD_TYPE}, but a variable named ${DEFAULT_LEVEL_VARIABLE_NAME} does not exists. Add one default to your configuration or presets")
        endif()

        # create the configurable cache value for this build and assign to its default
        set(${PROJECT_NAME}_CS_ASSERT_LEVEL_CONFIG_${CMAKE_BUILD_TYPE} "${${DEFAULT_LEVEL_VARIABLE_NAME}}"
            CACHE STRING
            "Configure the assert level for ${PROJECT_NAME}, use one of ${LEVEL_VALUES}")
        # TODO(sceriani) add drop down list for cmake gui

        # ensure assert level is valid
        _cs_assert_value_is_in_list("${${PROJECT_NAME}_CS_ASSERT_LEVEL_CONFIG_${CMAKE_BUILD_TYPE}}" "${LEVEL_VALUES}" "${PROJECT_NAME}_CS_ASSERT_LEVEL_CONFIG_${CMAKE_BUILD_TYPE}")

        # set CS_ASSERT_LEVEL so that file can be generated
        set(CS_ASSERT_LEVEL ${${PROJECT_NAME}_CS_ASSERT_LEVEL_CONFIG_${CMAKE_BUILD_TYPE}})

        # finally, configure the file directly, no need for intermediate generators
        cs_get_generate_folder_config(SPECIFIC_FOLDER)
        configure_file(
          "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../templates/ProjectAssert.hpp.in"
          "${SPECIFIC_FOLDER}/${PROJECT_NAME}Assert.hpp"
        )

    endif()
    message(STATUS "CMAKE_CONFIGURATION_TYPES ${CMAKE_CONFIGURATION_TYPES}, GENERATOR_IS_MULTI_CONFIG ${GENERATOR_IS_MULTI_CONFIG}, CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}")
    #message(FATAL_ERROR "stop here so far")


endfunction()