# Copyright (c) 2022 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(NOT "TARGET_FF_ARDUINO_UNO" IN_LIST MBED_TARGET_DEFINITIONS)
	set(TEST_SKIPPED "Target does not claim to implement the Arduino Uno form factor")
endif()

mbed_greentea_add_test(
	TEST_NAME
		mbed-hal-verify-arduino-pinmap
	TEST_SOURCES
		main.cpp
	TEST_SKIPPED
		${TEST_SKIPPED}
)