# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(NOT "DEVICE_RESET_REASON=1" IN_LIST MBED_TARGET_DEFINITIONS)
	set(TEST_SKIPPED "Reset Reason is not supported for this target")
endif()

if("TARGET_MIMXRT105X" IN_LIST MBED_TARGET_DEFINITIONS)
	# This test causes this target to die.  See https://github.com/mbed-ce/mbed-os/issues/83
	set(TEST_SKIPPED "Temporarily disabled for this target, see #83")
endif()

mbed_greentea_add_test(
    TEST_NAME
		mbed-hal-reset-reason
    TEST_SOURCES
        main.cpp
	HOST_TESTS_DIR
	    ${CMAKE_CURRENT_SOURCE_DIR}/../../host_tests
	TEST_SKIPPED
		${TEST_SKIPPED}
)
