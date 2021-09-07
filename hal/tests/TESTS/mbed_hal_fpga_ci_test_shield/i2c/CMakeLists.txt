# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

if(NOT "DEVICE_I2C=1" IN_LIST MBED_TARGET_DEFINITIONS)
    set(TEST_SKIPPED "I2C not supported for this target")
elseif(NOT "COMPONENT_FPGA_CI_TEST_SHIELD=1" IN_LIST MBED_TARGET_DEFINITIONS)
    set(TEST_SKIPPED "FPGA CI Test Shield is needed to run this test")
elseif(NOT("TARGET_FF_ARDUINO" IN_LIST MBED_TARGET_DEFINITIONS OR
    "TARGET_FF_ARDUINO_UNO" IN_LIST MBED_TARGET_DEFINITIONS) AND
    NOT DEFINED MBED_CONF_TARGET_DEFAULT_FORM_FACTOR)
    set(TEST_SKIPPED "Test not supported for this form factor")
endif()

list(
    APPEND
        TEST_REQ_LIBS_LIST
            mbed-fpga-ci-test-shield
            mbed-storage-blockdevice
)

mbed_greentea_add_test(
    TEST_NAME
        mbed-hal-fpga-ci-test-shield-i2c
    TEST_SOURCES
        main.cpp
    TEST_INCLUDE_DIRS
        .
    TEST_REQUIRED_LIBS
        ${TEST_REQ_LIBS_LIST}
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
