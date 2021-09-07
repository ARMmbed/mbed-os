# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(GoogleTest)

set(TEST_NAME lorawan-lorawan-timer-unittest)

add_executable(${TEST_NAME})

target_compile_definitions(${TEST_NAME}
    PRIVATE
        NDEBUG=1
        MBED_CONF_LORA_TX_MAX_SIZE=255
)

target_sources(${TEST_NAME}
    PRIVATE
        ${mbed-os_SOURCE_DIR}/connectivity/lorawan/system/LoRaWANTimer.cpp
        Test_LoRaWANTimer.cpp
)

target_link_libraries(${TEST_NAME}
    PRIVATE
        mbed-headers-events
        mbed-headers-platform
        mbed-headers-lorawan
        mbed-stubs
        mbed-stubs-headers
        gmock_main
)

gtest_discover_tests(${TEST_NAME} PROPERTIES LABELS "lorawan")
