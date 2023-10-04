# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_C_LIB} STREQUAL "small" OR MBED_GREENTEA_TEST_BAREMETAL)
    set(TEST_SKIPPED "Mutex test cases require RTOS with multithread to run")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-drivers-race-test
    TEST_SOURCES
        main.cpp
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
