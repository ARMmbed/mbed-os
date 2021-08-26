# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

if(NOT "TARGET_FF_ARDUINO_UNO" IN_LIST MBED_TARGET_DEFINITIONS)
    set(TEST_SKIPPED "Test needs Arduino Uno form factor")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-hal-pin-names-arduino_uno
    TEST_SOURCES
        main.cpp
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
