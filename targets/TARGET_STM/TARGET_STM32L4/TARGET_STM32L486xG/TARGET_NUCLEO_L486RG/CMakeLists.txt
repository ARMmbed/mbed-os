# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-l486rg INTERFACE)

target_sources(mbed-nucleo-l486rg
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(mbed-nucleo-l486rg
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-l486rg INTERFACE mbed-stm32l486xg)
