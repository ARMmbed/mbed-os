# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(NOT "DEVICE_FLASH=1" IN_LIST MBED_TARGET_DEFINITIONS)
	set(TEST_SKIPPED "Flash API not supported for this target")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-drivers-flashiap
    TEST_SOURCES
        main.cpp
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
