# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

list(
    APPEND
        TEST_SOURCE_LIST
            main.cpp
            test_c.c
            test_cpp.cpp
)

mbed_greentea_add_test(
    TEST_NAME
        mbed-platform-mbed-micro-static-assert
    TEST_SOURCES
        ${TEST_SOURCE_LIST}
)
