# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

if(NOT "DEVICE_TRNG=1" IN_LIST MBED_TARGET_DEFINITIONS)
    set(TEST_SKIPPED "TRNG API not supported for this target")
endif()

list(
    APPEND
        TEST_SOURCE_LIST
            main.cpp
            base64b/base64b.cpp
            pithy/pithy.c
)

list(
    APPEND
        TEST_INC_DIRS
            base64b
            pithy
)

mbed_greentea_add_test(
    TEST_NAME
        mbed-hal-trng
    TEST_SOURCES
        ${TEST_SOURCE_LIST}
    TEST_INCLUDE_DIRS
        ${TEST_INC_DIRS}
    TEST_REQUIRED_LIBS
        mbed-mbedtls
    HOST_TESTS_DIR
        "${CMAKE_CURRENT_LIST_DIR}/../../host_tests"
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
