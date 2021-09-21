# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

if(NOT "BLE" IN_LIST MBED_TARGET_LABELS)
    set(TEST_SKIPPED "BLE required")
elseif(MBED_GREENTEA_TEST_BAREMETAL)
    # Drivers for some BLE chips require RTOS which we need to check.
    set(TEST_SKIPPED "RTOS required")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-connectivity-ble-cordio-hci-driver
    TEST_SOURCES
        main.cpp
    TEST_REQUIRED_LIBS
        mbed-ble
        mbed-events
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
