# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

if(NOT "DEVICE_USBDEVICE=1" IN_LIST MBED_TARGET_DEFINITIONS)
    set(TEST_SKIPPED "USB Device not supported for this target")
elseif(NOT "USB_DEVICE_TESTS" IN_LIST MBED_CONFIG_DEFINITIONS)
    set(TEST_SKIPPED "USB Device tests not enabled")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-drivers-usb-basic
    TEST_SOURCES
        main.cpp
        USBEndpointTester.cpp
        USBTester.cpp
    TEST_REQUIRED_LIBS
        mbed-events
        mbed-usb
    HOST_TESTS_DIR
        "${CMAKE_CURRENT_LIST_DIR}/../../host_tests"
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
