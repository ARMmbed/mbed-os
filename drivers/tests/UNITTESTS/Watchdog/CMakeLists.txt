# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(GoogleTest)

set(TEST_NAME watchdog-unittest)

add_executable(${TEST_NAME})

target_compile_definitions(${TEST_NAME}
    PRIVATE
        DEVICE_WATCHDOG
        MBED_WDOG_ASSERT=1
)

target_sources(${TEST_NAME}
    PRIVATE
        ${mbed-os_SOURCE_DIR}/drivers/source/Watchdog.cpp
        test_watchdog.cpp
)

target_link_libraries(${TEST_NAME}
    PRIVATE
        mbed-headers-hal
        mbed-headers-drivers
        mbed-headers-platform
        mbed-stubs-platform
        mbed-stubs-hal
        gmock_main
)

gtest_discover_tests(${TEST_NAME} PROPERTIES LABELS "drivers")
