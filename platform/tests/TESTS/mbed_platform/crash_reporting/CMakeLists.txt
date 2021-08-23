# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

if(NOT DEFINED MBED_CONF_PLATFORM_CRASH_CAPTURE_ENABLED)
    set(TEST_SKIPPED "crash_reporting test not supported.")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-platform-crash-reporting
    TEST_SOURCES
        main.cpp
    HOST_TESTS_DIR
        "${CMAKE_CURRENT_LIST_DIR}/../../host_tests"
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
