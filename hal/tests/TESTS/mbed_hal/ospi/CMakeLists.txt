# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

if(NOT "DEVICE_OSPI=1" IN_LIST MBED_TARGET_DEFINITIONS)
    set(TEST_SKIPPED "OSPI not supported for this target")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-hal-ospi
    TEST_SOURCES
        main.cpp
        ospi_test_utils.cpp
    TEST_INCLUDE_DIRS
        .
        flash_configs
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
