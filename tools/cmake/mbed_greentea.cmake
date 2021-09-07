# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

option(MBED_GREENTEA_TEST_BAREMETAL "Select baremetal greentea tests" OFF)

set(MBED_HTRUN_ARGUMENTS "" CACHE STRING "Argument list to forward to htrun.")

# TODO: After we convert all greentea tests to use CTest, remove this code. We
# define these parameters in mbed-os/CMakeLists.txt for greentea tests.
if(NOT BUILD_GREENTEA_TESTS)
    set(MBED_CONFIG_PATH ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "")
    include(${CMAKE_CURRENT_LIST_DIR}/app.cmake)
    set(MBED_ROOT ${CMAKE_CURRENT_LIST_DIR}/../.. CACHE INTERNAL "")
endif()

# CMake Macro for generalizing CMake configuration across the greentea test suite with configurable parameters
# Macro args:
# TEST_NAME - Test suite name
# TEST_INCLUDE_DIRS - Test suite include directories for the test
# TEST_SOURCES - Test suite sources
# TEST_REQUIRED_LIBS - Test suite required libraries
# HOST_TESTS_DIR - Path to the "host_tests" directory
# TEST_SKIPPED - Reason if suite is skipped
#
# calling the macro:
# if(MBED_GREENTEA_TEST_BAREMETAL)
#     set(skip_reason "RTOS required")
# endif()
# mbed_greentea_add_test(
#     TEST_NAME
#         mbed-platform-system-reset
#     TEST_INCLUDE_DIRS
#         mbed_store
#     TEST_SOURCES
#         foo.cpp
#         bar.cpp
#     TEST_REQUIRED_LIBS
#         mbed-kvstore
#         mbed-xyz
#     HOST_TESTS_DIR
#         ${CMAKE_CURRENT_LIST_DIR}/host_tests
#     TEST_SKIPPED
#         ${skip_reason}
# )

function(mbed_greentea_add_test)
    set(options)
    set(singleValueArgs
        TEST_NAME
        TEST_SKIPPED
    )
    set(multipleValueArgs
        TEST_INCLUDE_DIRS
        TEST_SOURCES
        TEST_REQUIRED_LIBS
        HOST_TESTS_DIR
    )
    cmake_parse_arguments(MBED_GREENTEA
        "${options}"
        "${singleValueArgs}"
        "${multipleValueArgs}"
        ${ARGN}
    )

    if(NOT "${MBED_GREENTEA_TEST_SKIPPED}" STREQUAL "")
        add_test(
            NAME
                ${MBED_GREENTEA_TEST_NAME}
            COMMAND
                ${CMAKE_COMMAND} -E echo
                "Skipping ${MBED_GREENTEA_TEST_NAME}:"
                "${MBED_GREENTEA_TEST_SKIPPED}"
        )
        set_tests_properties(${MBED_GREENTEA_TEST_NAME}
            PROPERTIES
                SKIP_REGULAR_EXPRESSION "."
        )
        return()
    endif()

    # TODO: After we convert all greentea tests to use CTest, remove this
    # add_subdirectory call. We will attach the tests to the mbed-os project,
    # rather than creating a new project for each test that depends on mbed-os.
    if(NOT BUILD_GREENTEA_TESTS)
        add_subdirectory(${MBED_ROOT} build)
    endif()

    add_executable(${MBED_GREENTEA_TEST_NAME})

    target_include_directories(${MBED_GREENTEA_TEST_NAME}
        PRIVATE
            .
            ${MBED_GREENTEA_TEST_INCLUDE_DIRS}
    )

    target_sources(${MBED_GREENTEA_TEST_NAME}
        PRIVATE
            ${MBED_GREENTEA_TEST_SOURCES}
    )

    if(MBED_GREENTEA_TEST_BAREMETAL)
        list(APPEND MBED_GREENTEA_TEST_REQUIRED_LIBS mbed-baremetal)
    else()
        list(APPEND MBED_GREENTEA_TEST_REQUIRED_LIBS mbed-os)
    endif()

    list(APPEND MBED_GREENTEA_TEST_REQUIRED_LIBS greentea::client_userio mbed-greentea-io mbed-unity mbed-utest)

    target_link_libraries(${MBED_GREENTEA_TEST_NAME}
        PRIVATE
            ${MBED_GREENTEA_TEST_REQUIRED_LIBS}
    )

    mbed_set_post_build(${MBED_GREENTEA_TEST_NAME})

    if(NOT ${MBED_OUTPUT_EXT} STREQUAL "")
        set(MBED_GREENTEA_TEST_IMAGE_NAME "${MBED_GREENTEA_TEST_NAME}.${MBED_OUTPUT_EXT}")
    else()
        set(MBED_GREENTEA_TEST_IMAGE_NAME "${MBED_GREENTEA_TEST_NAME}.bin")
    endif()

    if(DEFINED MBED_GREENTEA_HOST_TESTS_DIR)
        list(APPEND MBED_HTRUN_ARGUMENTS "-e;${MBED_GREENTEA_HOST_TESTS_DIR}")
    endif()

    if(DEFINED MBED_TARGET)
        list(APPEND MBED_HTRUN_ARGUMENTS "-m;${MBED_TARGET}")
    endif()

    if(NOT "${MBED_GREENTEA_TEST_RESET_TIMEOUT}" STREQUAL "")
        list(APPEND MBED_HTRUN_ARGUMENTS "-R;${MBED_GREENTEA_TEST_RESET_TIMEOUT}")
    endif()

    list(APPEND CONFIG_DEFS_COPY ${MBED_CONFIG_DEFINITIONS})
    list(FILTER CONFIG_DEFS_COPY INCLUDE REGEX "MBED_CONF_PLATFORM_STDIO_BAUD_RATE")
    if(NOT ${CONFIG_DEFS_COPY} STREQUAL "")
        string(REGEX MATCH "[0-9]*$" BAUD_RATE ${CONFIG_DEFS_COPY})
        list(APPEND MBED_HTRUN_ARGUMENTS "--baud-rate=${BAUD_RATE}")
    endif()

    add_test(
        NAME ${MBED_GREENTEA_TEST_NAME}
        COMMAND mbedhtrun
            -f ${MBED_GREENTEA_TEST_IMAGE_NAME}
            ${MBED_HTRUN_ARGUMENTS}
        COMMAND_EXPAND_LISTS
    )

    option(VERBOSE_BUILD "Have a verbose build process")
    if(VERBOSE_BUILD)
        set(CMAKE_VERBOSE_MAKEFILE ON)
    endif()

endfunction()
