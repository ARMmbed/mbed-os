# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(NOT "DEVICE_SLEEP=1" IN_LIST MBED_TARGET_DEFINITIONS)
	set(TEST_SKIPPED "Sleep is not supported for this target")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-drivers-sleep-lock
    TEST_SOURCES
        main.cpp
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
