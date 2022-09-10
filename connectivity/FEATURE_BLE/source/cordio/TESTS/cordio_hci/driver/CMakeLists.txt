# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

mbed_greentea_add_test(
    TEST_NAME
        mbed-connectivity-ble-cordio-hci-driver
    TEST_SOURCES
        main.cpp
    TEST_REQUIRED_LIBS
        mbed-ble
)
