# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(GoogleTest)

set(TEST_NAME lorawan-loraphy-cn470-unittest)

add_executable(${TEST_NAME})

target_compile_definitions(${TEST_NAME}
    PRIVATE
        MBED_CONF_LORA_DOWNLINK_PREAMBLE_LENGTH=5
        MBED_CONF_LORA_TX_MAX_SIZE=255
        MBED_CONF_LORA_UPLINK_PREAMBLE_LENGTH=8
)

target_compile_options(${TEST_NAME}
    PRIVATE
        "-DMBED_CONF_LORA_FSB_MASK_CHINA={0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x00FF}"
        "-DMBED_CONF_LORA_FSB_MASK_CHINA={0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x00FF}"
)

target_sources(${TEST_NAME}
    PRIVATE
        ${mbed-os_SOURCE_DIR}/connectivity/lorawan/lorastack/phy/LoRaPHYCN470.cpp
        Test_LoRaPHYCN470.cpp
)

target_link_libraries(${TEST_NAME}
    PRIVATE
        mbed-headers-events
        mbed-headers-hal
        mbed-headers-platform
        mbed-headers-lorawan
        mbed-stubs
        mbed-stubs-headers
        mbed-stubs-lorawan
        gmock_main
)

gtest_discover_tests(${TEST_NAME} PROPERTIES LABELS "lorawan")
