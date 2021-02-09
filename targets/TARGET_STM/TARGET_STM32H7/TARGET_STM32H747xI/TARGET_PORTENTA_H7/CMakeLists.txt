# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-portenta-h7 INTERFACE)

target_sources(mbed-portenta-h7
    INTERFACE
        PeripheralPins.c
        system_clock_override.c
        portenta_power.cpp
)

target_include_directories(mbed-portenta-h7
    INTERFACE
        .
)

target_link_libraries(mbed-portenta-h7 INTERFACE mbed-stm32h747xi)
