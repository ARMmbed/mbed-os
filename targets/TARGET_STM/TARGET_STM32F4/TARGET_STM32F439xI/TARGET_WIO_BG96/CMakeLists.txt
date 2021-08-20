# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-wio-bg96 INTERFACE)

target_sources(mbed-wio-bg96
    INTERFACE
        PeripheralPins.c
        ONBOARD_QUECTEL_BG96.cpp
)

target_include_directories(mbed-wio-bg96
    INTERFACE
        .
)

target_link_libraries(mbed-wio-bg96 INTERFACE mbed-stm32f439xi)
