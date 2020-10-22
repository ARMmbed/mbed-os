# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-fpga-ci-test-shield
    INTERFACE
        include
        include/fpga_ci_test_shield
)

target_sources(mbed-fpga-ci-test-shield
    INTERFACE
        source/DynamicPinList.cpp
        source/I2CTester.cpp
        source/MbedTester.cpp
        source/SPIMasterTester.cpp
        source/SPISlaveTester.cpp
        source/SPITester.cpp
        source/UARTTester.cpp
)
