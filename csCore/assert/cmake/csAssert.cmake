include_guard(GLOBAL)
include(GNUInstallDirs)

function(_csCreateDefineName CS_ASSERT_PROJECT_NAME DEFINE_TOPIC DEFINE_NAME OUTPUT_NAME)
    set(FULL_NAME "${CS_ASSERT_PROJECT_NAME}_${DEFINE_TOPIC}_${DEFINE_NAME}")
    set(${OUTPUT_NAME} ${FULL_NAME} PARENT_SCOPE)
endfunction()

function(_csCreateDefineEnabledName CS_ASSERT_PROJECT_NAME DEFINE_TOPIC DEFINE_NAME OUTPUT_NAME)
    _csCreateDefineName(${CS_ASSERT_PROJECT_NAME} ${DEFINE_TOPIC} ${DEFINE_NAME} FULL_NAME)
    set(FULL_NAME "${FULL_NAME}_ENABLED")
    set(${OUTPUT_NAME} ${FULL_NAME} PARENT_SCOPE)
endfunction()

function(_csAssertDefaultOnForConfig PROJECT_NAME DEFINE_TOPIC DEFINE_NAME CONFIG)
    string(TOUPPER ${PROJECT_NAME} CS_ASSERT_PROJECT_NAME)
    set(${CS_ASSERT_PROJECT_NAME}_${DEFINE_TOPIC}_${DEFINE_NAME}_CONFIG_${CONFIG}_DEFAULT ON PARENT_SCOPE)
endfunction()

function(csAssertGenerateProject PROJECT_NAME TARGET_NAMESPACE)
    string(TOUPPER ${PROJECT_NAME} CS_ASSERT_PROJECT_NAME)

    # define default values mapping for different build types

    # in Debug enable DEBUG and DEFENSIVE
    _csAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT DEBUG Debug)
    _csAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT DEFENSIVE Debug)
    _csAssertDefaultOnForConfig(${PROJECT_NAME} CONTRACT DEBUG Debug)
    _csAssertDefaultOnForConfig(${PROJECT_NAME} CONTRACT DEFENSIVE Debug)

    # in RelWithDebInfo enable DEBUG only
    _csAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT DEBUG RelWithDebInfo)
    _csAssertDefaultOnForConfig(${PROJECT_NAME} CONTRACT DEBUG RelWithDebInfo)

    # no default enablement in other cases (e.g. Release)

    # get the relative path to the cmake source dir
    SET(CURRENT_RELATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
    cmake_path(RELATIVE_PATH CURRENT_RELATIVE_PATH BASE_DIRECTORY ${CMAKE_SOURCE_DIR})

    # lists to loop on
    set(DEFINE_TOPICS ASSERT CONTRACT)
    set(DEFINE_NAMES DEBUG DEFENSIVE PARANOID)

    get_property(GENERATOR_IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

    # defines a set of option to allow user to personalize the single assert level enablement
    if(GENERATOR_IS_MULTI_CONFIG)
        # multi config, create all single variables and add a default level based on the default defined above,
        foreach(DEFINE_TOPIC ${DEFINE_TOPICS})
            foreach(DEFINE_NAME ${DEFINE_NAMES})
                # retrieve the option name (equivalent to the define name)
                _csCreateDefineEnabledName(${CS_ASSERT_PROJECT_NAME} ${DEFINE_TOPIC} ${DEFINE_NAME} DEFINE_FULL_NAME_ENABLED)

                foreach(CONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})

                    # retrieve default value (if any)
                    _csCreateDefineName(${CS_ASSERT_PROJECT_NAME} ${DEFINE_TOPIC} ${DEFINE_NAME} DEFINE_BASE_NAME)
                    set(DEFAULT_VALUE_NAME "${DEFINE_BASE_NAME}_CONFIG_${CONFIG_TYPE}_DEFAULT")

                    if(${DEFAULT_VALUE_NAME})
                        set(DEFAULT_VALUE ${${DEFAULT_VALUE_NAME}})
                    else()
                        set(DEFAULT_VALUE OFF)
                    endif()
                    
                    # create a option in the cmake cache (configurable via GUI too)
                    option(${DEFINE_FULL_NAME_ENABLED}_CONFIG_${CONFIG_TYPE} "enable definition of ${DEFINE_FULL_NAME_ENABLED} for config ${CONFIG_TYPE}" ${DEFAULT_VALUE})
                endforeach()
            endforeach()
        endforeach()

    else()
        foreach(DEFINE_TOPIC ${DEFINE_TOPICS})
            foreach(DEFINE_NAME ${DEFINE_NAMES})
                # retrieve the option name (equivalent to the define name)
                _csCreateDefineEnabledName(${CS_ASSERT_PROJECT_NAME} ${DEFINE_TOPIC} ${DEFINE_NAME} DEFINE_FULL_NAME_ENABLED)

                # retrieve default value (if any)
                _csCreateDefineName(${CS_ASSERT_PROJECT_NAME} ${DEFINE_TOPIC} ${DEFINE_NAME} DEFINE_BASE_NAME)
                set(DEFAULT_VALUE_NAME "${DEFINE_BASE_NAME}_CONFIG_${CMAKE_BUILD_TYPE}_DEFAULT")

                if(${DEFAULT_VALUE_NAME})
                    set(DEFAULT_VALUE ${${DEFAULT_VALUE_NAME}})
                else()
                    set(DEFAULT_VALUE OFF)
                endif()
                
                # create a option in the cmake cache (configurable via GUI too)
                option(${DEFINE_FULL_NAME_ENABLED} "enable definition of ${DEFINE_FULL_NAME_ENABLED}" ${DEFAULT_VALUE})
            endforeach()
        endforeach()
    endif()

    # finally, generate the header file
    if(GENERATOR_IS_MULTI_CONFIG)
        
        # multi config, create all single variables and add a default level based on the default defined above,
        foreach(DEFINE_TOPIC ${DEFINE_TOPICS})
            foreach(DEFINE_NAME ${DEFINE_NAMES})
                # retrieve the option name (equivalent to the define name) and prepare value for enabled or not enabled case
                _csCreateDefineEnabledName(${CS_ASSERT_PROJECT_NAME} ${DEFINE_TOPIC} ${DEFINE_NAME} DEFINE_FULL_NAME)
                set(ENABLED_DEFINE "#define ${DEFINE_FULL_NAME}")
                set(NOT_ENABLED_DEFINE "// #define ${DEFINE_FULL_NAME}")

                set(EXPRESSION "")
                foreach(CONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})

                    if(${${DEFINE_FULL_NAME}_CONFIG_${CONFIG_TYPE}})
                        set(EXPRESSION "${EXPRESSION}$<$<CONFIG:${CONFIG_TYPE}>:${ENABLED_DEFINE}>")
                    else()
                        set(EXPRESSION "${EXPRESSION}$<$<CONFIG:${CONFIG_TYPE}>:${NOT_ENABLED_DEFINE}>")
                    endif()

                    
                endforeach()
                set(HEADER_DEFINITIONS "${HEADER_DEFINITIONS}\n${EXPRESSION}")
            endforeach()
            set(HEADER_DEFINITIONS "${HEADER_DEFINITIONS}\n\n")
        endforeach()

        # first, configure a file in a common directory
        cs_get_generate_folder_common(COMMON_FOLDER)
        configure_file(
          "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/templates/ProjectAssert.hpp.in"
          "${COMMON_FOLDER}/${PROJECT_NAME}Assert.hpp.in"
        )

        # then generate, to resolve generator expression according to the specific config
        cs_get_generate_folder_config(SPECIFIC_FOLDER)
        file(GENERATE
          OUTPUT "${SPECIFIC_FOLDER}/${CURRENT_RELATIVE_PATH}/${PROJECT_NAME}Assert.hpp"
          INPUT "${COMMON_FOLDER}/${PROJECT_NAME}Assert.hpp.in"
        )

        set(GENERATED_FILE "${SPECIFIC_FOLDER}/${CURRENT_RELATIVE_PATH}/${PROJECT_NAME}Assert.hpp")
        set(GENERATED_FILE_FOLDER "${SPECIFIC_FOLDER}")

    else()
        # text to be written in the header file, with all the definitions in place
        set(HEADER_DEFINITIONS "")
        foreach(DEFINE_TOPIC ${DEFINE_TOPICS})
            foreach(DEFINE_NAME ${DEFINE_NAMES})
                _csCreateDefineEnabledName(${CS_ASSERT_PROJECT_NAME} ${DEFINE_TOPIC} ${DEFINE_NAME} DEFINE_FULL_NAME)
                set(ENABLED_DEFINE "#define ${DEFINE_FULL_NAME}")
                set(NOT_ENABLED_DEFINE "// #define ${DEFINE_FULL_NAME}")

                if(${${DEFINE_FULL_NAME}})
                    set(HEADER_DEFINITIONS "${HEADER_DEFINITIONS}\n${ENABLED_DEFINE}")
                else()
                    set(HEADER_DEFINITIONS "${HEADER_DEFINITIONS}\n${NOT_ENABLED_DEFINE}")
                endif()
            endforeach()
            set(HEADER_DEFINITIONS "${HEADER_DEFINITIONS}\n\n")
        endforeach()

        # finally, configure the file directly, no need for intermediate generators
        cs_get_generate_folder_config(SPECIFIC_FOLDER)
        configure_file(
          "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/templates/ProjectAssert.hpp.in"
          "${SPECIFIC_FOLDER}/${CURRENT_RELATIVE_PATH}/${PROJECT_NAME}Assert.hpp"
        )

        set(GENERATED_FILE "${SPECIFIC_FOLDER}/${CURRENT_RELATIVE_PATH}/${PROJECT_NAME}Assert.hpp")
        set(GENERATED_FILE_FOLDER "${SPECIFIC_FOLDER}")
    endif()


    # define a target with the generate header
    set(TARGET_NAME ${PROJECT_NAME}Assert)
    cs_add_library(${TARGET_NAME} INTERFACE)
    cs_add_library(${TARGET_NAMESPACE}::${TARGET_NAME} ALIAS ${TARGET_NAME})

    cs_find_package(${TARGET_NAME} PUBLIC libassert REQUIRED CONFIG)

    cs_target_sources(${TARGET_NAME}
        PRIVATE_INSTALL_ABSOLUTE_PATH
            ${GENERATED_FILE}
        PRIVATE
        )

    cs_target_link_libraries(${TARGET_NAME}
        INTERFACE
            libassert::assert
    )

	target_include_directories(${TARGET_NAME}
    INTERFACE
      $<BUILD_INTERFACE:${GENERATED_FILE_FOLDER}>
    )

    # install to a default component
    cs_install_target(${TARGET_NAME} ${TARGET_NAME} DEFAULT)

    cs_finalize_target(${TARGET_NAME})

endfunction()

