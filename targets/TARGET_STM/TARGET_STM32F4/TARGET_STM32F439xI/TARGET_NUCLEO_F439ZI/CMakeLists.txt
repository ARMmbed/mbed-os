# Copyright (c) 2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(nucleo-f439zi INTERFACE)

target_sources(nucleo-f439zi
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(nucleo-f439zi
    INTERFACE
        .
)

target_link_libraries(nucleo-f439zi INTERFACE mbed-stm32f439xi)
