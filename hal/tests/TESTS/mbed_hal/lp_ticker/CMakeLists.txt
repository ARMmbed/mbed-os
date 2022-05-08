# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(NOT "DEVICE_LPTICKER=1" IN_LIST MBED_TARGET_DEFINITIONS)
	set(TEST_SKIPPED "Low-Power Ticker is not supported for this target")
endif()

mbed_greentea_add_test(
	TEST_NAME
		mbed-hal-lp-ticker
	TEST_SOURCES
		main.cpp
	HOST_TESTS_DIR
		${CMAKE_CURRENT_SOURCE_DIR}/../../host_tests
	TEST_SKIPPED
		${TEST_SKIPPED}
)