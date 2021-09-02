# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

if(NOT "DEVICE_USTICKER=1" IN_LIST MBED_TARGET_DEFINITIONS)
    set(TEST_SKIPPED "UsTicker need to be enabled for this test.")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-rtos-signals
    TEST_SOURCES
        main.cpp
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
