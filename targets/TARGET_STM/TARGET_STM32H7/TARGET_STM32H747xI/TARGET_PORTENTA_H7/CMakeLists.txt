# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(PORTENTA_H7 INTERFACE)

target_sources(PORTENTA_H7
    INTERFACE
        PeripheralPins.c
        system_clock_override.c
        portenta_power.cpp
)

target_include_directories(PORTENTA_H7
    INTERFACE
        .
)

target_link_libraries(PORTENTA_H7 INTERFACE STM32H747xI)
