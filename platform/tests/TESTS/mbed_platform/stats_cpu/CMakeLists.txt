# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

if(NOT DEFINED MBED_CPU_STATS_ENABLED OR NOT "DEVICE_LPTICKER=1" IN_LIST MBED_TARGET_DEFINITIONS
   OR NOT "DEVICE_SLEEP=1" IN_LIST MBED_TARGET_DEFINITIONS)
    set(TEST_SKIPPED "Stats cpu test not supported.")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-platform-stats-cpu
    TEST_SOURCES
        main.cpp
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
