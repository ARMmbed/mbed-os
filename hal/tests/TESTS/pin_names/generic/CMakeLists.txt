# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

# This test is enabled via CMake command-line argument -DMBED_HAL_PIN_NAMES_GENERIC_TEST=ON.
if(NOT MBED_HAL_PIN_NAMES_GENERIC_TEST)
    set(TEST_SKIPPED "Pin names generic test is not enabled for this Target")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-hal-pin-names-generic
    TEST_SOURCES
        main.cpp
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
