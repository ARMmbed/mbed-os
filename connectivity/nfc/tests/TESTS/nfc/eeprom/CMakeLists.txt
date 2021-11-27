# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

if(NOT "MBED_CONF_NFCEEPROM=1" IN_LIST MBED_CONFIG_DEFINITIONS)
    set(TEST_SKIPPED "NFC EEPROM required")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-connectivity-nfc-eeprom
    TEST_SOURCES
        main.cpp
    TEST_REQUIRED_LIBS
        mbed-nfc
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
