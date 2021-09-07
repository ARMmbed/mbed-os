# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(GoogleTest)

set(TEST_NAME lorawan-loramac-crypto-unittest)

add_executable(${TEST_NAME})

target_compile_definitions(${TEST_NAME}
    PRIVATE
        MBED_CONF_LORA_TX_MAX_SIZE=255
)

target_sources(${TEST_NAME}
    PRIVATE
        ${mbed-os_SOURCE_DIR}/connectivity/lorawan/lorastack/mac/LoRaMacCrypto.cpp
        Test_LoRaMacCrypto.cpp
)

target_link_libraries(${TEST_NAME}
    PRIVATE
        mbed-headers-platform
        mbed-headers-mbedtls
        mbed-headers-lorawan
        mbed-stubs
        mbed-stubs-headers
        mbed-stubs-mbedtls
        gmock_main
)

gtest_discover_tests(${TEST_NAME} PROPERTIES LABELS "lorawan")
