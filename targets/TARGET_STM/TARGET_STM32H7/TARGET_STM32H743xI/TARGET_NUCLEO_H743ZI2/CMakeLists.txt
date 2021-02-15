# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-nucleo-h743zi2 INTERFACE)

target_sources(mbed-nucleo-h743zi2
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(mbed-nucleo-h743zi2
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-h743zi2 INTERFACE mbed-stm32h743xi)
