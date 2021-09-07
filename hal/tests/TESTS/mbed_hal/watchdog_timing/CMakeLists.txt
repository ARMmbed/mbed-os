# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

if(NOT "DEVICE_WATCHDOG=1" IN_LIST MBED_TARGET_DEFINITIONS)
    set(TEST_SKIPPED "Watchdog not supported for this target")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-hal-watchdog-timing
    TEST_INCLUDE_DIRS
        .
    TEST_SOURCES
        main.cpp
    HOST_TESTS_DIR
        "${CMAKE_CURRENT_LIST_DIR}/../../host_tests"
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
