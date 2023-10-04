# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

mbed_greentea_add_test(
    TEST_NAME
        mbed-drivers-dev-null
    TEST_SOURCES
        main.cpp
    HOST_TESTS_DIR
		../../host_tests
)
