# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../../.. CACHE INTERNAL "")
set(TEST_TARGET mbed-drivers-mem-trace)

include(${MBED_PATH}/tools/cmake/mbed_greentea.cmake)

project(${TEST_TARGET})

if(NOT "MBED_MEM_TRACING_ENABLED=1" IN_LIST MBED_CONFIG_DEFINITIONS)
    set(TEST_SKIPPED "Memory tracing not enabled")
endif()

mbed_greentea_add_test(
    TEST_NAME
        ${TEST_TARGET}
    TEST_SOURCES
        main.cpp
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
