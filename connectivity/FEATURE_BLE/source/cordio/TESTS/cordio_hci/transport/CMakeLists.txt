# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("CORDIO_ZERO_COPY_HCI=1" IN_LIST MBED_CONFIG_DEFINITIONS)
    set(TEST_SKIPPED "Test not relevant for zero copy hci.")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-connectivity-ble-cordio-hci-transport
    TEST_SKIPPED
        ${TEST_SKIPPED}
    TEST_SOURCES
        main.cpp
    TEST_REQUIRED_LIBS
        mbed-ble
)
