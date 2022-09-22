# Copyright (c) 2022 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(NOT "DEVICE_USBDEVICE=1" IN_LIST MBED_TARGET_DEFINITIONS)
    set(TEST_SKIPPED "USB Device is not supported for this target")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-usb-device-basic
    TEST_SOURCES
        main.cpp
        USBEndpointTester.cpp
        USBTester.cpp
    HOST_TESTS_DIR
        "${CMAKE_CURRENT_LIST_DIR}/../../host_tests"
    TEST_SKIPPED
        ${TEST_SKIPPED}
    TEST_REQUIRED_LIBS
        mbed-usb
)