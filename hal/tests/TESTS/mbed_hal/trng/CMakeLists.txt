# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(NOT "DEVICE_TRNG=1" IN_LIST MBED_TARGET_DEFINITIONS)
	set(TEST_SKIPPED "True RNG is not supported for this target")
endif()

list(
    APPEND
        TEST_SOURCE_LIST
            main.cpp
            base64b/base64b.cpp
            pithy/pithy.c
)

list(
    APPEND
        TEST_INC_DIRS
            base64b
            pithy
)

mbed_greentea_add_test(
	TEST_NAME
		mbed-hal-trng
	TEST_SOURCES
		${TEST_SOURCE_LIST}
	TEST_INCLUDE_DIRS
        ${TEST_INC_DIRS}
	HOST_TESTS_DIR
		${CMAKE_CURRENT_SOURCE_DIR}/../../host_tests
	TEST_SKIPPED
		${TEST_SKIPPED}
	TEST_REQUIRED_LIBS
		mbed-mbedtls
)
