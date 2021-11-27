# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

if("TARGET_ARM_FM" IN_LIST MBED_TARGET_DEFINITIONS)
    set(TEST_SKIPPED "Wait ns test not supported.")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-platform-wait-ns
    TEST_SOURCES
        main.cpp
    HOST_TESTS_DIR
        "${CMAKE_CURRENT_LIST_DIR}/../../host_tests"
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
