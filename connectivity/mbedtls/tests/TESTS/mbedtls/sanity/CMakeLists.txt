# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if((NOT "FEATURE_EXPERIMENTAL_API=1" IN_LIST MBED_TARGET_DEFINITIONS)
	OR (NOT "FEATURE_PSA=1" IN_LIST MBED_TARGET_DEFINITIONS)
	OR (NOT "TARGET_MBED_PSA_SRV=1" IN_LIST MBED_TARGET_DEFINITIONS))
	set(TEST_SKIPPED "Only PSA targets (Arm-v7M) in emulation mode are supported.")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-connectivity-mbedtls-sanity
    TEST_SOURCES
        main.cpp
    TEST_REQUIRED_LIBS
        mbed-mbedtls
	TEST_SKIPPED
		${TEST_SKIPPED}
)
