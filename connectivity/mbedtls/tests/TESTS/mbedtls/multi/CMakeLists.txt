# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

mbed_greentea_add_test(
    TEST_NAME
        mbed-connectivity-mbedtls-multi
    TEST_SOURCES
        main.cpp
    TEST_REQUIRED_LIBS
        mbed-mbedtls
)
