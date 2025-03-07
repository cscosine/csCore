include_guard(GLOBAL)
include(GNUInstallDirs)

function(_csAssertCheckValueInList VALUE VALUE_LIST PROP_NAME)
    message(STATUS "_csAssertCheckValueInList list: ${VALUE_LIST}, value: ${VALUE}, propname: ${PROP_NAME}")
    list(FIND VALUE_LIST "${VALUE}" RES)
    if(RES EQUAL -1)
        message(FATAL_ERROR "${PROP_NAME} invalid value ${VALUE}, choose on of ${VALUE_LIST}")
    endif()
endfunction()

function(csAssertGenerateProject PROJECT_NAME)
    string(TOUPPER ${PROJECT_NAME} CS_ASSERT_PROJECT_NAME)

    # list of accepted values
    set(LEVEL_VALUES NONE DEBUG DEFENSIVE PARANOID)

    # add a variable to the project 
    set(${PROJECT_NAME}_CS_ASSERT_LEVEL_CONFIG_DEBUG "DEFENSIVE"
        CACHE STRING
        "Configure the assert level for ${PROJECT_NAME}, use one of ${LEVEL_VALUES}")
    set(${PROJECT_NAME}_CS_ASSERT_LEVEL_CONFIG_RELEASE  "NONE"
        CACHE STRING
        "Configure the assert level for ${PROJECT_NAME}, use one of ${LEVEL_VALUES}")

    # ensure check is in one the valid values
    _csAssertCheckValueInList("${${PROJECT_NAME}_CS_ASSERT_LEVEL_CONFIG_DEBUG}" "${LEVEL_VALUES}" "${PROJECT_NAME}_CS_ASSERT_LEVEL_CONFIG_DEBUG")
    _csAssertCheckValueInList("${${PROJECT_NAME}_CS_ASSERT_LEVEL_CONFIG_RELEASE}" "${LEVEL_VALUES}" "${PROJECT_NAME}_CS_ASSERT_LEVEL_CONFIG_RELEASE")

    # retrieve the option value and use it to generate the assert level
    set(P_RELEASE "${${PROJECT_NAME}_CS_ASSERT_LEVEL_CONFIG_RELEASE}") # helper variable
    set(P_DEBUG "${${PROJECT_NAME}_CS_ASSERT_LEVEL_CONFIG_DEBUG}") # helper variable
    set(CS_ASSERT_LEVEL "$<$<CONFIG:Debug>:${P_DEBUG}>$<$<CONFIG:Release>:${P_RELEASE}>")

    # first, configure a file in build directory
    configure_file(
    "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../templates/ProjectAssert.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Assert.hpp.in"
    )

    # then generate, to resolve generator expression according to the specific config
    file(GENERATE
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Assert.hpp"
    INPUT "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Assert.hpp.in"
    )


endfunction()