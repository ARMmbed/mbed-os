# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

if(NOT "DEVICE_USTICKER=1" IN_LIST MBED_TARGET_DEFINITIONS OR
    NOT "DEVICE_SLEEP=1" IN_LIST MBED_TARGET_DEFINITIONS)
    set(TEST_SKIPPED "test not supported")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-hal-sleep-manager-racecondition
    TEST_INCLUDE_DIRS
        .
    TEST_SOURCES
        main.cpp
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
