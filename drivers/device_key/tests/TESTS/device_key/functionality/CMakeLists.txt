# Copyright (c) 2023 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(NOT "DEVICE_TRNG=1" IN_LIST MBED_TARGET_DEFINITIONS)
	set(TEST_SKIPPED "True RNG is not supported for this target so device key cannot be used")
endif()

if(NOT "DEVICE_FLASH=1" IN_LIST MBED_TARGET_DEFINITIONS)
	set(TEST_SKIPPED "Flash IAP is not supported for this target so device key cannot be used")
endif()

mbed_greentea_add_test(
	TEST_NAME
		mbed-device_key-functionality
	TEST_SOURCES
		main.cpp
    TEST_REQUIRED_LIBS
        mbed-device_key
	TEST_SKIPPED
		${TEST_SKIPPED}
)