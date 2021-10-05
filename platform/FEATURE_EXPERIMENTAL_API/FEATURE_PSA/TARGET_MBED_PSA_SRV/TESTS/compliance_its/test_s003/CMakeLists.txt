# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../../ CACHE INTERNAL "")
set(TEST_TARGET mbed-platform-psa-compliance-its-s003)

include(${MBED_PATH}/tools/cmake/mbed_greentea.cmake)

project(${TEST_TARGET})

mbed_greentea_add_test(
    TEST_NAME
        ${TEST_TARGET}
    TEST_SOURCES
        main.c
        test_entry.c
        test_s003.c
    TEST_REQUIRED_LIBS
        mbed-psa-tal
)
