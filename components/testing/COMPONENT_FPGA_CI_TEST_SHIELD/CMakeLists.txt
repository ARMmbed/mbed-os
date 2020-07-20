# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os
    PRIVATE
        DynamicPinList.cpp
        I2CTester.cpp
        MbedTester.cpp
        SPIMasterTester.cpp
        SPISlaveTester.cpp
        SPITester.cpp
        UARTTester.cpp
)

target_include_directories(mbed-os
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
