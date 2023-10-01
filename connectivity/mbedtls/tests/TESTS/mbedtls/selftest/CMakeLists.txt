# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(MBED_GREENTEA_TEST_BAREMETAL)
    set(TEST_SKIPPED "This test requires an RTOS!")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-connectivity-mbedtls-selftest
    TEST_SOURCES
        main.cpp
    TEST_REQUIRED_LIBS
        mbed-mbedtls
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
