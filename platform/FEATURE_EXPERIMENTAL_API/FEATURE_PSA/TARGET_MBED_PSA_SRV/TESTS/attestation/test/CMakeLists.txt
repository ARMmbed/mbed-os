# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

if(MBED_GREENTEA_TEST_BAREMETAL)
    set(TEST_SKIPPED "RTOS required")
endif()

mbed_greentea_add_test(
    TEST_NAME 
        mbed-platform-psa-attestation
    TEST_SOURCES
        main.cpp
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
