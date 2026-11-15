include_guard(GLOBAL)
include(GNUInstallDirs)

# Important note: if you change the defaults of the assert values (eg csSetAssertDefaultOnForConfig) you need to remove the BUILD directory and build from scratch, otherwise the generated headers are not updated

function(_csCreateDefineName CS_ASSERT_PROJECT_NAME DEFINE_TOPIC DEFINE_NAME OUTPUT_NAME)
    set(FULL_NAME "${CS_ASSERT_PROJECT_NAME}_${DEFINE_TOPIC}_${DEFINE_NAME}")
    set(${OUTPUT_NAME} ${FULL_NAME} PARENT_SCOPE)
endfunction()

function(_csCreateDefineEnabledName CS_ASSERT_PROJECT_NAME DEFINE_TOPIC DEFINE_NAME OUTPUT_NAME)
    _csCreateDefineName(${CS_ASSERT_PROJECT_NAME} ${DEFINE_TOPIC} ${DEFINE_NAME} FULL_NAME)
    set(FULL_NAME "${FULL_NAME}_ENABLED")
    set(${OUTPUT_NAME} ${FULL_NAME} PARENT_SCOPE)
endfunction()

function(csSetAssertDefaultOnForConfig PROJECT_NAME DEFINE_TOPIC DEFINE_NAME CONFIG VALUE)
    string(TOUPPER ${PROJECT_NAME} CS_ASSERT_PROJECT_NAME)
    set(${CS_ASSERT_PROJECT_NAME}_${DEFINE_TOPIC}_${DEFINE_NAME}_CONFIG_${CONFIG}_DEFAULT "${VALUE}" CACHE BOOL "${DEFINE_TOPIC} ${DEFINE_NAME} default value" FORCE)
endfunction()

function(csAssertGenerateProject PROJECT_NAME TARGET_NAMESPACE)
    string(TOUPPER ${PROJECT_NAME} CS_ASSERT_PROJECT_NAME)

    # define default values mapping for different (standard) build types

    # in Release enable only RELEASE
    csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT RELEASE Release ON)
    csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT RELWITHDEBINFO Release OFF)
    csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT DEBUG Release OFF)
    csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT PARANOID Release OFF)
    csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT SAFE_MATH Release OFF)

    # in Debug enable RELEASE, RELWITHDEBINFO and DEBUG
    csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT RELEASE Debug ON)
    csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT RELWITHDEBINFO Debug ON)
    csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT DEBUG Debug ON)
    csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT PARANOID Debug OFF)
    csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT SAFE_MATH Debug OFF)

    # in RelWithDebInfo enable RELEASE and RELWITHDEBINFO
    csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT RELEASE RelWithDebInfo ON)
    csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT RELWITHDEBINFO RelWithDebInfo ON)
    csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT DEBUG RelWithDebInfo OFF)
    csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT PARANOID RelWithDebInfo OFF)
    csSetAssertDefaultOnForConfig(${PROJECT_NAME} ASSERT SAFE_MATH RelWithDebInfo OFF)

    # get the relative path to the cmake source dir
    SET(CURRENT_RELATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
    cmake_path(RELATIVE_PATH CURRENT_RELATIVE_PATH BASE_DIRECTORY ${CMAKE_SOURCE_DIR})

    # lists to loop on
    set(DEFINE_TOPICS ASSERT) # so far only ASSERT topic, but keep the list so that if we need to add a new topic (e.g. CONTRACTS) everything is already in place
    set(DEFINE_NAMES RELEASE RELWITHDEBINFO DEBUG PARANOID SAFE_MATH)

    get_property(GENERATOR_IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

    # defines a set of option to allow user to personalize the single assert level enablement
    if(GENERATOR_IS_MULTI_CONFIG)
        # multi config, create all single variables and add a default level based on the default defined above,
        foreach(DEFINE_TOPIC ${DEFINE_TOPICS})
            foreach(DEFINE_NAME ${DEFINE_NAMES})
                # retrieve the option name (equivalent to the define name)
                _csCreateDefineEnabledName(${CS_ASSERT_PROJECT_NAME} ${DEFINE_TOPIC} ${DEFINE_NAME} DEFINE_FULL_NAME_ENABLED)

                foreach(CONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})

                    # retrieve default value
                    _csCreateDefineName(${CS_ASSERT_PROJECT_NAME} ${DEFINE_TOPIC} ${DEFINE_NAME} DEFINE_BASE_NAME)
                    set(DEFAULT_VALUE_NAME "${DEFINE_BASE_NAME}_CONFIG_${CONFIG_TYPE}_DEFAULT")

                    if(DEFINED ${DEFAULT_VALUE_NAME})
                        set(DEFAULT_VALUE ${${DEFAULT_VALUE_NAME}})
                    else()
                        message(FATAL_ERROR "no default value for configuration ${CONFIG_TYPE}, variable ${DEFINE_BASE_NAME}_CONFIG_${CONFIG_TYPE}_DEFAULT does not exist, maybe you forgot to use csSetAssertDefaultOnForConfig for ${CONFIG_TYPE} before calling csAssertGenerateProject")
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

    # finally, generate the header (templates/ProjectAssert.hpp.in): the same for every configuration (no
    # per-configuration source, which the Xcode generator does not support), the levels enabled are the compile
    # definitions <PROJECT>_ASSERT_<LEVEL>_ENABLED of the target, per configuration (below); the header defines their
    # <...>_ENABLED_VALUE (true / false). A new level: here (DEFINE_NAMES) and in the template.
    set(CS_ASSERT_NAMESPACE ${PROJECT_NAME})

    set(ASSERT_COMPILE_DEFINITIONS "")
    foreach(DEFINE_TOPIC ${DEFINE_TOPICS})
        foreach(DEFINE_NAME ${DEFINE_NAMES})
            _csCreateDefineEnabledName(${CS_ASSERT_PROJECT_NAME} ${DEFINE_TOPIC} ${DEFINE_NAME} DEFINE_FULL_NAME)

            # the configurations of the level: $<$<CONFIG:c1,c2,...>:<define>>, evaluated in the configuration of the
            # build, also for the consumers of the installed package (in their own configuration)
            set(ENABLED_CONFIGS "")
            if(GENERATOR_IS_MULTI_CONFIG)
                foreach(CONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})
                    if(${${DEFINE_FULL_NAME}_CONFIG_${CONFIG_TYPE}})
                        list(APPEND ENABLED_CONFIGS ${CONFIG_TYPE})
                    endif()
                endforeach()
            else()
                # single config: the option of CMAKE_BUILD_TYPE, the defaults of the other configurations; every
                # configuration is installed in the same package (one targets file), so the definitions must not
                # depend on the configuration installed last
                _csCreateDefineName(${CS_ASSERT_PROJECT_NAME} ${DEFINE_TOPIC} ${DEFINE_NAME} DEFINE_BASE_NAME)
                if(${${DEFINE_FULL_NAME}})
                    list(APPEND ENABLED_CONFIGS ${CMAKE_BUILD_TYPE})
                endif()
                get_cmake_property(CACHE_VARIABLES CACHE_VARIABLES)
                foreach(CACHE_VARIABLE ${CACHE_VARIABLES})
                    if(CACHE_VARIABLE MATCHES "^${DEFINE_BASE_NAME}_CONFIG_(.+)_DEFAULT$")
                        set(CONFIG_TYPE ${CMAKE_MATCH_1})
                        if(NOT CONFIG_TYPE STREQUAL CMAKE_BUILD_TYPE AND ${CACHE_VARIABLE})
                            list(APPEND ENABLED_CONFIGS ${CONFIG_TYPE})
                        endif()
                    endif()
                endforeach()
            endif()
            if(ENABLED_CONFIGS)
                list(SORT ENABLED_CONFIGS)
                list(JOIN ENABLED_CONFIGS "," ENABLED_CONFIGS)
                list(APPEND ASSERT_COMPILE_DEFINITIONS "$<$<CONFIG:${ENABLED_CONFIGS}>:${DEFINE_FULL_NAME}>")
            endif()
        endforeach()
    endforeach()

    cs_get_generated_dir(GENERATED_FILE_FOLDER)
    set(GENERATED_FILE "${GENERATED_FILE_FOLDER}/${CURRENT_RELATIVE_PATH}/${PROJECT_NAME}Assert.hpp")
    configure_file("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/templates/ProjectAssert.hpp.in" "${GENERATED_FILE}")

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

    # the levels enabled, per configuration (see the header above)
    target_compile_definitions(${TARGET_NAME} INTERFACE ${ASSERT_COMPILE_DEFINITIONS})

	target_include_directories(${TARGET_NAME}
    INTERFACE
      $<BUILD_INTERFACE:${GENERATED_FILE_FOLDER}>
    )

    # install to a default component
    cs_install_target(${TARGET_NAME} COMPONENT ${TARGET_NAME} DEFAULT)

    # compile each single header in standalone, ensures includes are self contained
    cs_target_check_installed_headers(${TARGET_NAME} COMPILE_PROFILES cscmake_strict_warnings)

endfunction()
