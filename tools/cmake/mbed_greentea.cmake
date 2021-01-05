# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(MBED_CONFIG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/.mbedbuild CACHE INTERNAL "")

include(${MBED_PATH}/tools/cmake/app.cmake)

# CMake Macro for generalizing CMake configuration across the greentea test suite with configurable parameters
# Macro args:
# TEST_NAME - Test suite name
# TEST_INCLUDE_DIRS - Test suite include directories for the test
# TEST_SOURCES - Test suite sources
# TEST_REQUIRED_LIBS - Test suite required libraries
# 
# calling the macro:
# mbed_greentea_cmake_macro(
#    TEST_NAME mbed-platform-system-reset
#    TEST_INCLUDE_DIRS mbed_store
#    TEST_SOURCES foo.cpp bar.cpp
#    TEST_REQUIRED_LIBS mbed-kvstore mbed-xyz
# )

macro(mbed_greentea_cmake_macro)
    set(options)
    set(singleValueArgs TEST_NAME)
    set(multipleValueArgs
        TEST_INCLUDE_DIRS
        TEST_SOURCES
        TEST_REQUIRED_LIBS
    )
    cmake_parse_arguments(MBED_GREENTEA
        "${options}"
        "${singleValueArgs}"
        "${multipleValueArgs}"
        ${ARGN}
    )

    set(TEST_NAME ${MBED_GREENTEA_TEST_NAME})

    add_subdirectory(${MBED_PATH} build)

    add_executable(${TEST_NAME})

    mbed_configure_app_target(${TEST_NAME})

    mbed_set_mbed_target_linker_script(${TEST_NAME})

    target_include_directories(${TEST_NAME}
        PRIVATE
            .
            ${MBED_GREENTEA_TEST_INCLUDE_DIRS}
    )

    target_sources(${TEST_NAME}
        PRIVATE
            main.cpp
            ${MBED_GREENTEA_TEST_SOURCES}
    )

    if(MBED_BAREMETAL_GREENTEA_TEST)
        list(APPEND MBED_GREENTEA_TEST_REQUIRED_LIBS mbed-baremetal mbed-greentea)
    else()
        list(APPEND MBED_GREENTEA_TEST_REQUIRED_LIBS mbed-os mbed-greentea)
    endif()

    target_link_libraries(${TEST_NAME}
        PRIVATE
            ${MBED_GREENTEA_TEST_REQUIRED_LIBS}
    )

    mbed_set_post_build(${TEST_NAME})

    option(VERBOSE_BUILD "Have a verbose build process")
    if(VERBOSE_BUILD)
        set(CMAKE_VERBOSE_MAKEFILE ON)
    endif()

endmacro()
