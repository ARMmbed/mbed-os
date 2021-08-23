# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

mbed_greentea_add_test(
    TEST_NAME
        mbed-platform-filehandle
    TEST_INCLUDE_DIRS
        .
    TEST_SOURCES
        main.cpp
)
