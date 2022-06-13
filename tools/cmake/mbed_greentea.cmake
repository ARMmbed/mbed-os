# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

option(MBED_GREENTEA_TEST_BAREMETAL "Select baremetal greentea tests" OFF)

set(MBED_HTRUN_ARGUMENTS "" CACHE STRING "Argument list to forward to htrun.")
set(MBED_GREENTEA_SERIAL_PORT "" CACHE STRING "Serial port name to talk to the Mbed device on.  Should look like 'COM3' on Windows or '/dev/ttyACM1' on Linux.")

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

    if("${MBED_GREENTEA_SERIAL_PORT}" STREQUAL "")
        message(FATAL_ERROR "Will not be able to run greentea tests without MBED_GREENTEA_SERIAL_PORT defined!")
    endif()

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

    # User can set this cache variable to supply extra arguments to greentea.
    # such as: -d to set the drive path
    list(APPEND MBED_HTRUN_ARGUMENTS -p ${MBED_GREENTEA_SERIAL_PORT})

    if(DEFINED MBED_GREENTEA_EXTRA_HTRUN_ARGUMENTS)
        list(APPEND MBED_HTRUN_ARGUMENTS ${MBED_GREENTEA_EXTRA_HTRUN_ARGUMENTS})
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

    # Configure the CMake script which runs the test.
    # We have to use a helper script in order to run both the flashing and htrun in one test
    configure_file(
        ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/mbed-run-greentea-test.in.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/mbed-run-greentea-${MBED_GREENTEA_TEST_NAME}.cmake
        @ONLY)

    # Make sure the cmake script gets removed later
    set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${CMAKE_CURRENT_BINARY_DIR}/mbed-run-greentea-${MBED_GREENTEA_TEST_NAME}.cmake)

    add_test(
        NAME ${MBED_GREENTEA_TEST_NAME}
        COMMAND
            ${CMAKE_COMMAND} -P mbed-run-greentea-${MBED_GREENTEA_TEST_NAME}.cmake
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )


endfunction()
