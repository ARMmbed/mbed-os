# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(GoogleTest)

set(TEST_NAME analogin-unittest)

add_executable(${TEST_NAME})

target_compile_definitions(${TEST_NAME}
    PRIVATE
        DEVICE_ANALOGIN
        MBED_CONF_TARGET_DEFAULT_ADC_VREF=3.3f
)

target_sources(${TEST_NAME}
    PRIVATE
        ${mbed-os_SOURCE_DIR}/drivers/source/AnalogIn.cpp
        test_analogin.cpp
)

target_link_libraries(${TEST_NAME}
    PRIVATE
        mbed-headers-platform
        mbed-headers-hal
        mbed-headers-drivers
        mbed-stubs-hal
        mbed-stubs-platform
)

gtest_discover_tests(${TEST_NAME} PROPERTIES LABELS "drivers")
