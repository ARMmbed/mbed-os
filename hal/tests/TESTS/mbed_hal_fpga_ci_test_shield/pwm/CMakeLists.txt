# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

list(
    APPEND
        TEST_REQ_LIBS_LIST
            mbed-fpga-ci-test-shield
            mbed-storage-blockdevice
)

mbed_greentea_add_test(
    TEST_NAME
        mbed-hal-fpga-ci-test-shield-pwm
    TEST_SOURCES
        main.cpp
    TEST_INCLUDE_DIRS
        .
    TEST_REQUIRED_LIBS
        ${TEST_REQ_LIBS_LIST}
)
