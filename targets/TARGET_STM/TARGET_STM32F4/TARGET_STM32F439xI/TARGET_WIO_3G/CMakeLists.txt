# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-wio-3g INTERFACE)

target_sources(mbed-wio-3g
    INTERFACE
        PeripheralPins.c
        ONBOARD_QUECTEL_UG96.cpp
)

target_include_directories(mbed-wio-3g
    INTERFACE
        .
)

target_link_libraries(mbed-wio-3g INTERFACE mbed-stm32f439xi)
