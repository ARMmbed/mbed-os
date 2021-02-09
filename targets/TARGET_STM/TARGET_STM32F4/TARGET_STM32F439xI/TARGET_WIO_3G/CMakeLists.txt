# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(wio-3g INTERFACE)

target_sources(wio-3g
    INTERFACE
        PeripheralPins.c
        system_clock.c
        ONBOARD_QUECTEL_UG96.cpp
)

target_include_directories(wio-3g
    INTERFACE
        .
)

target_link_libraries(wio-3g INTERFACE mbed-stm32f439xi)
