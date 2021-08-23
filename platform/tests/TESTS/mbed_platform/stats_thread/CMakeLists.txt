# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

if(NOT DEFINED MBED_THREAD_STATS_ENABLED)
    set(TEST_SKIPPED "Thread stats test not supported.")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-platform-stats-thread
    TEST_SOURCES
        main.cpp
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
