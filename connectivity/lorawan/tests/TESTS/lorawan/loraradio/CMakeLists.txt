# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(mbed_greentea)

set(SUPPORTED_LABELS ${MBED_TARGET_LABELS})
list(FILTER SUPPORTED_LABELS INCLUDE REGEX "^(SX126X|SX1272|SX1276|STM32WL)$")

if(SUPPORTED_LABELS STREQUAL "")
    set(TEST_SKIPPED "Lora radio is not configured")
endif()

mbed_greentea_add_test(
    TEST_NAME
        mbed-connectivity-lorawan-loraradio
    TEST_SOURCES
        main.cpp
    TEST_REQUIRED_LIBS
        mbed-lorawan
    TEST_SKIPPED
        ${TEST_SKIPPED}
)
