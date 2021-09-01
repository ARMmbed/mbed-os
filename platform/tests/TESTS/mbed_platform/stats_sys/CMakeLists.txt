# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

if(NOT DEFINED MBED_SYS_STATS_ENABLED)
    set(TEST_SKIPPED "System stats test not supported.")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-platform-stats-sys
    TEST_SOURCES
        main.cpp
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
