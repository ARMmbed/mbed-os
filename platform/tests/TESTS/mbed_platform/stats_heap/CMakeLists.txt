# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

if(NOT DEFINED MBED_HEAP_STATS_ENABLED)
    set(TEST_SKIPPED "Stats heap test not supported.")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-platform-stats-heap
    TEST_SOURCES
        main.cpp
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
