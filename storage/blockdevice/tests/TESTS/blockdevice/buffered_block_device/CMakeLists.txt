# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

mbed_greentea_add_test(
    TEST_NAME
        mbed-storage-blockdevice-buffered_block_device
    TEST_SOURCES
        main.cpp
    TEST_REQUIRED_LIBS
        mbed-storage-blockdevice
)
