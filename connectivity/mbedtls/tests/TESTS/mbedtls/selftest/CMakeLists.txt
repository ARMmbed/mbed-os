# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

if(NOT "MBEDTLS_SELF_TEST" IN_LIST MBED_CONFIG_DEFINITIONS)
    set(TEST_SKIPPED "MBEDTLS_SELF_TEST undefined")
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
