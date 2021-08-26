# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

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
)
