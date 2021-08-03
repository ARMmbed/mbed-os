# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(GoogleTest)

set(TEST_NAME sfdp-unittest)

add_executable(${TEST_NAME})

target_compile_definitions(${TEST_NAME}
    PRIVATE
        DEVICE_SPI
)

target_sources(${TEST_NAME}
    PRIVATE
        ${mbed-os_SOURCE_DIR}/storage/blockdevice/source/SFDP.cpp
        test_sfdp.cpp
)

target_link_libraries(${TEST_NAME}
    PRIVATE
        mbed-headers-blockdevice
        mbed-headers-platform
        mbed-stubs-platform
        gmock_main
)

gtest_discover_tests(${TEST_NAME} PROPERTIES LABELS "storage")
