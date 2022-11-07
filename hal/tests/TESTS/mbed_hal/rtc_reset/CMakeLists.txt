# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(NOT "DEVICE_RTC=1" IN_LIST MBED_TARGET_DEFINITIONS)
	set(TEST_SKIPPED "RTC is not supported for this target")
endif()

if(MBED_TARGET STREQUAL "MIMXRT1050_EVK")
	# This test causes this target to die.  See https://github.com/mbed-ce/mbed-os/issues/83
	set(TEST_SKIPPED "Temporarily disabled for this target, see #83")
endif()

mbed_greentea_add_test(
	TEST_NAME
		mbed-hal-rtc-reset
	TEST_SOURCES
		main.cpp
	HOST_TESTS_DIR
		${CMAKE_CURRENT_SOURCE_DIR}/../../host_tests
	TEST_SKIPPED
		${TEST_SKIPPED}
)